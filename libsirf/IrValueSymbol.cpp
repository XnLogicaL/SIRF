// This file is a part of the SIRF (Simple Intermediate Representation Format)
// project Copyright (C) 2025 XnLogical - Licensed under GNU GPL v3.0

#include "IrValueSymbol.hpp"

namespace sirf {

constexpr bool IrValueSymbol::isLvalue() const {
  return false;
}

std::string IrValueSymbol::toString() const {
  return '@' + id;
}

} // namespace sirf
