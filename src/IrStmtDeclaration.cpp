// This file is a part of the SIRF (Simple Intermediate Representation Format)
// project Copyright (C) 2025 XnLogical - Licensed under GNU GPL v3.0

#include <IR/IrStmtDeclaration.hpp>

namespace SIRF {

std::string IrStmtDeclaration::toString() const {
  return SIRF::toString(kind) + ' ' + value->toString() + '\n';
}

std::string toString(IrDeclKind kind) {
  using enum IrDeclKind;

  // clang-format off
  switch (kind) {
  case EXTERN: return "extern";
  case GLOBAL: return "global";
  default:     return "";
  } // clang-format on

  SIRF_UNREACHABLE();
}

} // namespace SIRF
