// This file is a part of the SIRF (Simple Intermediate Representation Format)
// project Copyright (C) 2025 XnLogical - Licensed under GNU GPL v3.0

#include "IrValueLiteral.hpp"

namespace sirf {

bool IrValueLiteral::isSigned() const {
  return false;
}

constexpr bool IrValueLiteral::isLvalue() const {
  return false;
}

std::string IrValueLiteral::toString() const {
  return std::to_string(value) + ' ' + type->toString();
}

} // namespace sirf
