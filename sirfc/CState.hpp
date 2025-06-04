// This file is a part of the SIRF (Simple Intermediate Representation Format)
// project Copyright (C) 2025 XnLogical - Licensed under GNU GPL v3.0

#ifndef SIRFC_CSTATE_HPP
#define SIRFC_CSTATE_HPP

#include <Core/Common.hpp>
#include <IR/IrHolder.hpp>

namespace sirf {

struct Token;
using TokenHolder = std::vector<Token>;

struct CState {
  int exitCode = 0;
  IrHolder irHolder;
  TokenHolder tokHolder;
  std::string inputPath;
  std::string inputSource;
  std::string outputPath;
};

} // namespace sirf

#endif
