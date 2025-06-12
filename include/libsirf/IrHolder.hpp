// This file is a part of the SIRF (Simple Intermediate Representation Format)
// project Copyright (C) 2025 XnLogical - Licensed under GNU GPL v3.0

#ifndef SIRF_IRHOLDER_HPP
#define SIRF_IRHOLDER_HPP

#include "Common.hpp"
#include "IrStmtBase.hpp"

namespace sirf {

class IrHolder final {
public:
  std::vector<IrStmt> data;
};

} // namespace sirf

#endif
