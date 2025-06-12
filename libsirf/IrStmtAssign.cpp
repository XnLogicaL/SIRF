// This file is a part of the SIRF (Simple Intermediate Representation Format)
// project Copyright (C) 2025 XnLogical - Licensed under GNU GPL v3.0

#include "IrStmtAssign.hpp"

namespace sirf {

std::string IrStmtAssign::toString() const {
  return lvalue->toString() + " = " + rvalue->toString() + '\n';
}

} // namespace sirf
