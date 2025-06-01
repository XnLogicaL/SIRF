// This file is a part of the SIRF (Simple Intermediate Representation Format)
// project Copyright (C) 2025 XnLogical - Licensed under GNU GPL v3.0

// STL
#include <iostream>
#include <fstream>

// SIRF
#include <Asm/AsmGen.hpp>
#include <IR/IrValueLiteral.hpp>
#include <IR/IrTypeSized.hpp>
#include <IR/IrTypePtr.hpp>

// spdlog
#include <spdlog/spdlog.h>

// argparse
#include <argparse/argparse.hpp>

int main(int argc, char** argv) {
  using namespace SIRF;

  spdlog::set_pattern("%^%l:%$ %v");

  argparse::ArgumentParser parser("sirfc");
  parser.add_argument("target").help("Target SIRF IR file").required();

  try {
    parser.parse_args(argc, argv);
  }
  catch (const std::exception& err) {
    spdlog::error(err.what());
    return 1;
  }

  std::string filePath = parser.get("target");
  std::string fileSource;

  std::ifstream ifs(filePath);
  std::string line;

  if (!ifs.is_open()) {
    spdlog::error("target: Failed to open file '{}'", filePath);
    return 1;
  }

  while (std::getline(ifs, line)) {
    fileSource += line + '\n';
  }

  std::cout << fileSource;

  IrHolder holder;


  AsmGenerator gen(holder);
  std::cout << gen.generate();

  return 0;
}
