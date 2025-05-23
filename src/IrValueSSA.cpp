// This file is a part of the SIRF (Simple Intermediate Representation Format)
// project Copyright (C) 2025 XnLogical - Licensed under GNU GPL v3.0

#include <IR/IrValueSSA.hpp>

namespace SIRF {

constexpr bool IrValueSSA::isLvalue() const {
  return true;
}

std::string IrValueSSA::toString() const {
  using enum IrSSAKind;

  if (kind == NUMBERED)
    return '%' + std::to_string(value.number);
  else
    return '%' + std::string(value.name);

  SIRF_UNREACHABLE();
}

} // namespace SIRF
