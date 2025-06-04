// This file is a part of the SIRF (Simple Intermediate Representation Format)
// project Copyright (C) 2025 XnLogical - Licensed under GNU GPL v3.0

#ifndef SIRF_IRVALUELITERAL_HPP
#define SIRF_IRVALUELITERAL_HPP

#include <Core/Common.hpp>
#include <IR/IrTypeBase.hpp>
#include <IR/IrValueBase.hpp>

namespace sirf {

class IrValueLiteral final : public IrValueBase {
public:
  const IrType type;
  const uint64_t value;

  explicit IrValueLiteral(IrType type, uint64_t val)
    : type(type),
      value(val) {}

  static inline IrValue newValue(IrType type, uint64_t val) {
    return std::make_shared<IrValueLiteral>(type, val);
  }

  bool isSigned() const;

  /// Returns whether if the IR literal value can be treated as an lvalue
  constexpr bool isLvalue() const override;

  /// Returns the string representation of the IR literal value
  std::string toString() const override;
};

} // namespace sirf

#endif
