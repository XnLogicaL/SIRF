// This file is a part of the SIRF (Simple Intermediate Representation Format)
// project Copyright (C) 2025 XnLogical - Licensed under GNU GPL v3.0

#ifndef SIRF_IRSTMTBASE_HPP
#define SIRF_IRSTMTBASE_HPP

#include "Common.hpp"

namespace sirf {

class IrStmtBase {
public:
  virtual std::string toString() const = 0;
};

using IrStmt = IrStmtBase*;
using IrScope = std::vector<IrStmt>;

} // namespace sirf

#endif
