// This file is a part of the SIRF (Simple Intermediate Representation Format)
// project Copyright (C) 2025 XnLogical - Licensed under GNU GPL v3.0

#ifndef SIRF_IRVALUELABEL_HPP
#define SIRF_IRVALUELABEL_HPP

#include "Common.hpp"
#include "IrValueBase.hpp"

namespace sirf {

class IrValueLabel final : public IrValueBase {
public:
  const std::string id;

  explicit IrValueLabel(std::string id)
    : id(id) {}

  /// Returns whether if the Ir value can be interpreted as an lvalue
  constexpr bool isLvalue() const override;

  /// Returns the string representation of the Ir value
  std::string toString() const override;
};

} // namespace sirf

#endif
