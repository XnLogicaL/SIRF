// This file is a part of the SIRF (Simple Intermediate Representation Format)
// project Copyright (C) 2025 XnLogical - Licensed under GNU GPL v3.0

#include "IrValuePtr.hpp"

namespace sirf {

constexpr bool IrValuePtr::isLvalue() const {
  return false;
}

std::string IrValuePtr::toString() const {
  return "ptr " + val->toString();
}

} // namespace sirf
