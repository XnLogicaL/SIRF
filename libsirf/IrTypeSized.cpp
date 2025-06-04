// This file is a part of the SIRF (Simple Intermediate Representation Format)
// project Copyright (C) 2025 XnLogical - Licensed under GNU GPL v3.0

#include <IR/IrTypeSized.hpp>

namespace sirf {

std::string IrTypeSized::toString() const {
  return (isSigned ? 'i' : 'u') + std::to_string(size);
}

} // namespace sirf
