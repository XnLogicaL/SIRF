// This file is a part of the SIRF (Simple Intermediate Representation Format)
// project Copyright (C) 2025 XnLogical - Licensed under GNU GPL v3.0

#ifndef SIRF_IRVALUEBASE_HPP
#define SIRF_IRVALUEBASE_HPP

#include <Core/Common.hpp>

namespace SIRF {

class IrValueBase {
public:
  virtual ~IrValueBase() = default;

  /// Returns whether if the IR value be treated as an lvalue
  virtual constexpr bool isLvalue() const = 0;

  /// Returns the string representation of the IR value
  virtual std::string toString() const = 0;
};

using IrValue = std::shared_ptr<IrValueBase>;

} // namespace SIRF

#endif
