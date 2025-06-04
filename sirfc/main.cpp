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
  as,
  exe,
  ttree,
  ttree_json,
};

static void listEmitTargets() {
  spdlog::info("Emission targets:");
  std::cout << "  -e as                   raw assembly (platform specific)\n"
            << "  -e exe                  linked executable binary (platform specific)\n"
            << "  -e ttree                tokenized representation\n"
            << "  -e ttree_json           tokenized representation in json format\n";
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
  case EmitTarget::as:
    return ".s";
  case EmitTarget::exe:
#if defined(__linux__) || defined(__apple__)
    return ".out";
#elif defined(_WIN32)
  return ".exe"
#endif
  case EmitTarget::ttree:
    return ".tt";
  case EmitTarget::ttree_json:
    return ".json";
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
  auto spath = state.inputPath + ".s";
  auto opath = state.inputPath + ".o";

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

template<bool emitJSON>
static constexpr int emitTtree(sirf::CState& state) {
  std::ofstream ofs(state.outputPath);

  if constexpr (emitJSON) {
    ofs << "[\n";
  }

  const auto emitTokenJSON = [&](const sirf::Token& tok, bool is_last) {
    ofs << "  { \"kind\": \"" << magic_enum::enum_name(tok.kind) << "\", \"lexeme\": \"" << tok.lexeme << "\", \"pos\": " << tok.loc << " }";
    if (!is_last)
      ofs << ",";
    ofs << "\n";
  };

  const auto emitTokenTup = [&](const sirf::Token& tok) {
    ofs << '(' << std::setw(12) << std::left << magic_enum::enum_name(tok.kind) << ", '" << tok.lexeme << "', " << std::setw(3) << std::setfill('0')
        << tok.loc << ")\n";
  };

  const std::size_t count = state.tokHolder.size();
  for (std::size_t i = 0; i < count; ++i) {
    const auto& tok = state.tokHolder[i];
    if constexpr (emitJSON)
      emitTokenJSON(tok, i + 1 == count);
    else
      emitTokenTup(tok);
  }

  if constexpr (emitJSON) {
    ofs << "]\n";
  }

  return 0;
}

static constexpr int _main_impl(int argc, char** argv) {
  spdlog::set_pattern("sirfc: %^%l:%$ %v");

  // clang-format off
  argparse::ArgumentParser cli("sirfc");

  cli.add_argument("input")
    .required()
    .help("target source file containing SIRF IR (text format)");
  
  cli.add_argument("-o", "--output")
    .default_value("a")
    .nargs(1)
    .help("target output file path");
  
  cli.add_argument("-e", "--emit")
    .default_value("exe")
    .nargs(1)
    .choices("list", "as", "exe", "ttree", "ttree_json")
    .help("emission target type");

  cli.add_argument("-V", "--verbose")
    .flag()
    .help("enable verbose output");
  
  cli.add_argument("-no-clean", "--no-cleanup")
    .flag()
    .help("disable intermediate compilation artifact cleaning");
  
  cli.add_argument("-O", "--optimize")
    .default_value(0)
    .nargs(1)
    .choices(0, 1, 2, 3)
    .help("optimization level");
  // clang-format on

  try {
    cli.parse_args(argc, argv);
  }
  catch (const std::exception& err) {
    spdlog::error(err.what());
    return 1;
  }

  // CLI flags
  bool fverbose = cli.get<bool>("-v");
  bool fnoclean = cli.get<bool>("-no-clean");

  EmitTarget et = magic_enum::enum_cast<EmitTarget>(cli.get("-e")).value_or(EmitTarget::none);
  if (et == EmitTarget::none) {
    spdlog::error("invalid emit target");
    return 1;
  }

  sirf::CState state;
  state.inputPath = cli.get("input");
  state.outputPath = cli.get("-o");

  // check for default output path
  if (state.outputPath == "a") {
    state.outputPath += getTargetExtension(et);
  }

  sirf::IrLexer lexer(state);
  sirf::IrParser parser(state);

  // read input file
  std::ifstream ifs(state.inputPath);
  if (!ifs.is_open()) {
    spdlog::error("target: failed to open file '{}'", state.inputPath);
    return 1;
  }

  std::string line;
  while (std::getline(ifs, line)) {
    state.inputSource += line + '\n';
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
  case EmitTarget::as:
    return emitAssembly(state);
  case EmitTarget::ttree:
    return emitTtree<false>(state);
  case EmitTarget::ttree_json:
    return emitTtree<true>(state);
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
