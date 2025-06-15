// This file is a part of the SIRF (Simple Intermediate Representation Format)
// project Copyright (C) 2025 XnLogical - Licensed under GNU GPL v3.0

#include "IrStmtData.hpp"

namespace sirf {

std::string IrStmtData::toString() const {
  std::ostringstream oss;
  oss << "data " << id.toString() << ": ";

  for (size_t i = 0; const uint8_t chr : buf) {
    oss << std::format("{:x}", chr);
    if (i < buf.size() - 1) {
      oss << ", ";
    }
  }

  return oss.str();
}

} // namespace sirf
