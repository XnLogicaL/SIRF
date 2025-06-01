// This file is a part of the SIRF (Simple Intermediate Representation Format)
// project Copyright (C) 2025 XnLogical - Licensed under GNU GPL v3.0

#include <IR/IrTypePtr.hpp>

namespace SIRF {

std::string IrTypePtr::toString() const {
  return type->toString() + '^';
}

} // namespace SIRF
