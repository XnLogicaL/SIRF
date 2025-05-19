// This file is a part of the SIRF (Simple Intermediate Representation Format)
// project Copyright (C) 2025 XnLogical - Licensed under GNU GPL v3.0

#ifndef SIRF_IRVALUEBASE_HPP
#define SIRF_IRVALUEBASE_HPP

#include <string>
#include <SIRF/Core/APIConfig.hpp>

namespace SIRF {

class IrValueBase {
public:
  /// Returns whether if the IR value be treated as an lvalue
  virtual constexpr bool isLvalue() const;

  /// Returns the string representation of the IR value
  virtual std::string toString() const;
};

} // namespace SIRF

#endif
