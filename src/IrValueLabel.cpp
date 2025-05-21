// This file is a part of the SIRF (Simple Intermediate Representation Format)
// project Copyright (C) 2025 XnLogical - Licensed under GNU GPL v3.0

#include <SIRF/IR/IrValueLabel.hpp>

namespace SIRF {

constexpr bool IrValueLabel::isLvalue() const {
  return false;
}

std::string IrValueLabel::toString() const {
  return '.' + id;
}

} // namespace SIRF
