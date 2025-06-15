// This file is a part of the SIRF (Simple Intermediate Representation Format)
// project Copyright (C) 2025 XnLogical - Licensed under GNU GPL v3.0

#ifndef SIRF_IRTYPEBASE_HPP
#define SIRF_IRTYPEBASE_HPP

#include "Common.hpp"

namespace sirf {

class IrTypeBase {
public:
  virtual std::string toString() const = 0;
};

using IrType = IrTypeBase*;

} // namespace sirf

#endif
