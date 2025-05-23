// This file is a part of the SIRF (Simple Intermediate Representation Format)
// project Copyright (C) 2025 XnLogical - Licensed under GNU GPL v3.0

#include <IR/IrValueSymbol.hpp>

namespace SIRF {

constexpr bool IrValueSymbol::isLvalue() const {
  return false;
}

std::string IrValueSymbol::toString() const {
  return '@' + id;
}

} // namespace SIRF
