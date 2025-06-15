// This file is a part of the SIRF (Simple Intermediate Representation Format)
// project Copyright (C) 2025 XnLogical - Licensed under GNU GPL v3.0

#ifndef SIRFC_CSTATE_HPP
#define SIRFC_CSTATE_HPP

#include "Common.hpp"
#include "IrHolder.hpp"
#include <arena/arena.h>

namespace sirf {

struct Token;
using TokenHolder = std::vector<Token>;

struct CState {
  int exitCode = 0;
  IrHolder irHolder;
  TokenHolder tokHolder;
  ArenaAllocator al{1024 * 1024 * 8};
  std::string inputPath;
  std::string inputSource;
  std::string outputPath;
};

} // namespace sirf

#endif
