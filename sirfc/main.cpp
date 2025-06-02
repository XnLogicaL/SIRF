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

#define ASM_PATH(path) program.get<bool>("-exec") ? path + ".s" : outPath

template<typename... Args>
static int executeCommand(const std::format_string<Args...> fmt, Args&&... args) {
  const std::string cmd = std::format(fmt, std::forward<Args>(args)...);
  spdlog::info(cmd);
  return std::system(cmd.c_str());
}

int main(int argc, char** argv) {
  using namespace SIRF;

  spdlog::set_pattern("%^%l:%$ %v");
  argparse::ArgumentParser program("sirfc");
  program.add_argument("input").required();
  program.add_argument("-o", "--output").required().metavar("output").nargs(1);

  program.add_argument("-ttree", "--token-tree").flag();
  program.add_argument("-asm", "--assembly").flag();
  program.add_argument("-exec", "--generate-executable").flag();
  program.add_argument("-no-clean", "--no-cleanup").flag();

  try {
    program.parse_args(argc, argv);
  }
  catch (const std::exception& err) {
    spdlog::error(err.what());
    return 1;
  }

  std::string outPath = program.get("-o");
  std::string inPath = program.get("input");
  std::string fileSource;
  std::ifstream ifs(inPath);
  std::string line;

  CState state{false, inPath, fileSource};
  IrLexer lexer(state);
  IrParser parser(state);
  AsmGenerator gen(state.irHolder);

  if (!ifs.is_open()) {
    spdlog::error("target: Failed to open file '{}'", inPath);
    return 1;
  }

  while (std::getline(ifs, line)) {
    fileSource += line + '\n';
  }

  lexer.tokenize();
  parser.parse();

  if (program.get<bool>("-ttree")) {
    for (const Token& tok : state.tokHolder) {
      std::cout << '(' << (int)tok.kind << ", " << tok.lexeme << ", " << tok.loc << ")\n";
    }
  }

  if (state.fail) {
    return 1;
  }

  std::string assembly = gen.generate();

  if (program.get<bool>("-asm")) {
    std::cout << assembly << "\n";
  }

  std::ofstream ofs(ASM_PATH(inPath));
  ofs << assembly;
  ofs.close();

  if (program.get<bool>("-exec")) {
#ifndef __linux__
    spdlog::error("flag '-exec' is only supported on linux");
    return 1;
#endif

#ifdef __x86_64__
    std::string format = "elf64";
#else
    spdlog::error("flag '-exec' is only supported on x86-64");
    return 1;
#endif

    if (int code = executeCommand("nasm -f{} {} -o {}", format, ASM_PATH(inPath), inPath + ".o");
        code != 0) {
      spdlog::error("assembler returned non-zero exit code");
      return 1;
    }

    if (int code = executeCommand("gcc -no-pie {} -o {}", inPath + ".o", outPath); code != 0) {
      spdlog::error("linker returned non-zero exit code");
      return 1;
    }

    if (!program.get<bool>("-no-clean")) {
      if (int code = executeCommand("rm {} && rm {}", ASM_PATH(inPath), inPath + ".o"); code != 0) {
        spdlog::error("cleanup command returned non-zero exit code");
        return 1;
      }
    }
  }

  return 0;
}
