// This file is a part of the SIRF (Simple Intermediate Representation Format)
// project Copyright (C) 2025 XnLogical - Licensed under GNU GPL v3.0

#include "IrTypeNamed.hpp"

namespace sirf {

std::string IrTypeNamed::toString() const {
  return sirf::toString(kind);
}

std::string toString(IrNamedTypeKind kind) {
  using enum IrNamedTypeKind;

  // clang-format off
  switch (kind) {
  case VOID:  return "void";
  case BOOL:  return "bool";
  case BYTE:  return "byte";
  case WORD:  return "word";
  case DWORD: return "dword";
  case QWORD: return "qword";
  default:    return "";
  } // clang-format on

  SIRF_UNREACHABLE();
}

} // namespace sirf
