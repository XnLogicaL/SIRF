// This file is a part of the SIRF (Simple Intermediate Representation Format)
// project Copyright (C) 2025 XnLogical - Licensed under GNU GPL v3.0

#ifndef SIRF_IRVALUESSA_HPP
#define SIRF_IRVALUESSA_HPP

#include <Core/Common.hpp>
#include <IR/IrValueBase.hpp>

namespace sirf {

class IrValueSSA final : public IrValueBase {
public:
  const size_t id;

  explicit IrValueSSA(size_t id)
    : id(id) {}

  static inline IrValue newValue(size_t id) {
    return std::make_shared<IrValueSSA>(id);
  }

  /// Returns whether if the Ir value can be interpreted as an lvalue
  constexpr bool isLvalue() const override;

  /// Returns the string representation of the Ir value
  std::string toString() const override;
};

} // namespace sirf

#endif
