// This file is a part of the SIRF (Simple Intermediate Representation Format)
// project Copyright (C) 2025 XnLogical - Licensed under GNU GPL v3.0

#include <IR/IrStmtLabel.hpp>

namespace sirf {

std::string IrStmtLabel::toString() const {
  return label.toString() + ":\n";
}

} // namespace sirf
