// This file is a part of the SIRF (Simple Intermediate Representation Format)
// project Copyright (C) 2025 XnLogical - Licensed under GNU GPL v3.0

#include <IR/IrAttribute.hpp>

namespace SIRF {

std::string IrAttribute::toString() const {
  return '!' + id;
}

} // namespace SIRF
