// This file is a part of the SIRF (Simple Intermediate Representation Format)
// project Copyright (C) 2025 XnLogical - Licensed under GNU GPL v3.0

#include <iostream>
#include <fstream>
#include <cstdlib>

#include <Asm/AsmGen.hpp>
#include <IR/IrValueLiteral.hpp>
#include <IR/IrTypeSized.hpp>
#include <IR/IrTypePtr.hpp>
#include "IrLexer.hpp"
#include "IrParser.hpp"

#include <spdlog/spdlog.h>
#include <argparse/argparse.hpp>

#define ASM_PATH(path) path + ".s"
#define OBJ_PATH(path) path + ".o"

int main(int argc, char** argv) {
  using namespace SIRF;

  spdlog::set_pattern("%^%l:%$ %v");
  argparse::ArgumentParser program("sirfc");
  program.add_argument("input").required();
  program.add_argument("-o", "--output").required().metavar("output").nargs(1);

  program.add_argument("-ttree", "--token-tree").flag();
  program.add_argument("-asm", "--assembly").flag();
  program.add_argument("-exec", "--generate-executable").flag();

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

  if (!ifs.is_open()) {
    spdlog::error("target: Failed to open file '{}'", inPath);
    return 1;
  }

  while (std::getline(ifs, line)) {
    fileSource += line + '\n';
  }

  IrHolder irHolder;
  TokenHolder tokHolder;

  IrLexer lexer(fileSource, tokHolder);
  lexer.tokenize();

  if (program.get<bool>("-ttree")) {
    for (const Token& tok : tokHolder) {
      std::cout << '(' << (int)tok.kind << ", " << tok.lexeme << ", " << tok.loc << ")\n";
    }
  }

  IrParser parser(fileSource, tokHolder, irHolder);
  parser.parse();

  AsmGenerator gen(irHolder);
  auto asms = gen.generate();

  if (program.get<bool>("-asm")) {
    std::cout << asms << "\n";
  }

  std::ofstream ofs(ASM_PATH(inPath));
  ofs << asms;
  ofs.close();

  if (program.get<bool>("-exec")) {
#ifndef __linux__
    spdlog::error("flag '-exec' is only supported on linux");
    return 1;
#endif

#ifdef __x86_64__
    const char* format = "elf64";
#else
    spdlog::error("flag '-exec' is only supported on x86-64");
    return 1;
#endif

    auto assembleCmd =
      std::format("nasm -f{} {} -o {}", format, ASM_PATH(inPath), OBJ_PATH(inPath));
    auto linkCmd = std::format("gcc -no-pie {} -o {}", OBJ_PATH(inPath), outPath);
    auto cleanupCmd = std::format("rm {} && rm {}", ASM_PATH(inPath), OBJ_PATH(inPath));

    if (int code = std::system(assembleCmd.c_str()); code != 0) {
      spdlog::error("assembly command failed");
      return 1;
    }

    if (int code = std::system(linkCmd.c_str()); code != 0) {
      spdlog::error("link command failed");
      return 1;
    }

    if (int code = std::system(cleanupCmd.c_str()); code != 0) {
      spdlog::error("cleanup command failed");
      return 1;
    }
  }

  return 0;
}
