// This file is a part of the SIRF (Simple Intermediate Representation Format)
// project Copyright (C) 2025 XnLogical - Licensed under GNU GPL v3.0

#ifndef SIRF_IRTYPEBASE_HPP
#define SIRF_IRTYPEBASE_HPP

#include <Core/Common.hpp>

namespace SIRF {

class IrTypeBase {
public:
  virtual ~IrTypeBase() = default;

  virtual std::string toString() const;
};

using IrType = std::shared_ptr<IrTypeBase>;

} // namespace SIRF

#endif
