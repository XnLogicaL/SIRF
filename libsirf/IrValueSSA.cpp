// This file is a part of the SIRF (Simple Intermediate Representation Format)
// project Copyright (C) 2025 XnLogical - Licensed under GNU GPL v3.0

#include "IrValueSSA.hpp"

namespace sirf {

constexpr bool IrValueSSA::isLvalue() const {
  return true;
}

std::string IrValueSSA::toString() const {
  return '%' + std::to_string(id);
}

} // namespace sirf
