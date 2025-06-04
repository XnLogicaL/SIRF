// This file is a part of the SIRF (Simple Intermediate Representation Format)
// project Copyright (C) 2025 XnLogical - Licensed under GNU GPL v3.0

#include <iostream>
#include <fstream>
#include <cstdlib>

#include <Asm/AsmGen.hpp>
#include <IR/IrValueLiteral.hpp>
#include <IR/IrTypeSized.hpp>
#include <IR/IrTypePtr.hpp>
#include "CState.hpp"
#include "IrLexer.hpp"
#include "IrParser.hpp"

#include <spdlog/spdlog.h>
#include <argparse/argparse.hpp>
#include <magic_enum/magic_enum.hpp>

enum class EmitTarget {
  none = 0,
  list,
  asm_,
  exe,
  ttree,
};

static void listEmitTargets() {
  spdlog::info("Emission targets:");
  std::cout << "  asm      raw assembly (platform specific)\n"
            << "  exe      linked executable binary (platform specific)\n"
            << "  ttree    tokenized representation\n";
}

template<typename... Args>
static int executeCommand(const std::format_string<Args...> fmt, Args&&... args) {
  const std::string cmd = std::format(fmt, std::forward<Args>(args)...);
  spdlog::info(cmd);
  return std::system(cmd.c_str());
}

static constexpr const char* getBinFormat() {
  constexpr size_t WORD_SIZE = sizeof(uintptr_t);

#ifdef __linux__
  return WORD_SIZE == 4 ? "elf32" : "elf64";
#endif

  return "<sirf-error-bin-format-not-found>";
}

static constexpr const char* getTargetExtension(EmitTarget target) {
  switch (target) {
  case EmitTarget::asm_:
    return ".s";
  case EmitTarget::exe:
#if defined(__linux__) || defined(__apple__)
    return ".out";
#elif defined(_WIN32)
    return ".exe"
#endif
  default:
    break;
  }

  return "<sirf-error-target-ext-not-found>";
}

static constexpr int emitAssembly(sirf::CState& state) {
  sirf::AsmGenerator gen(state.irHolder);
  std::ofstream ofs(state.outputPath);
  ofs << gen.generate();
  ofs.close();

  return 0;
}

static constexpr int emitExecutable(sirf::CState& state, bool fnoclean) {
  auto spath = state.filePath + ".s";
  auto opath = state.filePath + ".o";

  sirf::AsmGenerator gen(state.irHolder);
  std::ofstream ofs(spath);
  ofs << gen.generate();
  ofs.close();

  int result = 0;
  if (int code = executeCommand("nasm -f{} {} -o {}", getBinFormat(), spath, opath); code != 0) {
    spdlog::error("assembler returned non-zero exit code");
    result = 1;
    goto cleanup;
  }

  if (int code = executeCommand("gcc -no-pie {} -o {}", opath, state.outputPath); code != 0) {
    spdlog::error("linker returned non-zero exit code");
    result = 1;
    goto cleanup;
  }

cleanup:
  if (fnoclean == false) {
    // We dont care about the status of the remove command
    executeCommand("rm {} && rm {}", spath, opath);
  }

  return result;
}

static constexpr int emitTtree(sirf::CState& state) {
  for (const sirf::Token& tok : state.tokHolder) {
    // clang-format off
    std::cout << '(' << std::setw(12) << std::setfill(' ') << std::left << magic_enum::enum_name(tok.kind) << ", '"
      << tok.lexeme << "', "
      << std::setw(3) << std::setfill('0') << tok.loc << ")\n";
    // clang-format on
  }

  return 0;
}

static constexpr int _main_impl(int argc, char** argv) {
  spdlog::set_pattern("sirfc: %^%l:%$ %v");

  argparse::ArgumentParser program("sirfc");
  // core args
  program.add_argument("input").required();
  program.add_argument("-o", "--output").default_value("a").nargs(1);
  program.add_argument("-e", "--emit").default_value("exe").nargs(1);

  // flags
  program.add_argument("-v", "--verbose").flag();
  program.add_argument("-no-clean", "--no-cleanup").flag();

  try {
    program.parse_args(argc, argv);
  }
  catch (const std::exception& err) {
    spdlog::error(err.what());
    return 1;
  }

  // CLI flags
  bool fverbose = program.get<bool>("-v");
  bool fnoclean = program.get<bool>("-no-clean");

  EmitTarget et = magic_enum::enum_cast<EmitTarget>(program.get("-e")).value_or(EmitTarget::none);
  if (et == EmitTarget::none) {
    spdlog::error("invalid emit target");
    return 1;
  }

  sirf::CState state;
  state.filePath = program.get("input");
  state.outputPath = program.get("-o");

  // check for default output path
  if (state.outputPath == "a") {
    state.outputPath += getTargetExtension(et);
  }

  sirf::IrLexer lexer(state);
  sirf::IrParser parser(state);

  // read input file
  std::ifstream ifs(state.filePath);
  if (!ifs.is_open()) {
    spdlog::error("target: failed to open file '{}'", state.filePath);
    return 1;
  }

  std::string line;
  while (std::getline(ifs, line)) {
    state.fileSource += line + '\n';
  }

  // lex & parse input file
  lexer.tokenize();
  parser.parse();

  if (state.exitCode != 0) {
    if (fverbose)
      spdlog::error("parser returned non-zero exit code");
    return 1;
  }

  // code emission
  switch (et) {
  // special case: list is not an actual target type
  case EmitTarget::list:
    listEmitTargets();
    break;
  case EmitTarget::exe:
    return emitExecutable(state, fnoclean);
  case EmitTarget::asm_:
    return emitAssembly(state);
  case EmitTarget::ttree:
    return emitTtree(state);
  default:
    spdlog::error("unknown emit target\n");
    listEmitTargets();
    return 1;
  }

  return 0;
}

int main(int argc, char** argv) {
  try {
    return _main_impl(argc, argv);
  }
  catch (const std::exception& err) {
    return 2;
  }
}
