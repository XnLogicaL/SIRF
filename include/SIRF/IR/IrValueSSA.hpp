// This file is a part of the SIRF (Simple Intermediate Representation Format)
// project Copyright (C) 2025 XnLogical - Licensed under GNU GPL v3.0

#ifndef SIRF_IRVALUESSA_HPP
#define SIRF_IRVALUESSA_HPP

#include <Core/Common.hpp>
#include <IR/IrValueBase.hpp>

namespace SIRF {

enum class IrSSAKind {
  NUMBERED,
  NAMED,
};

union IrSSAUn {
  uint32_t number;
  const char* name;
};

class IrValueSSA final : public IrValueBase {
public:
  const IrSSAKind kind;
  const IrSSAUn value;

  explicit IrValueSSA(IrSSAKind kind, IrSSAUn un)
    : kind(kind),
      value(un) {}

  /// Returns whether if the Ir value can be interpreted as an lvalue
  constexpr bool isLvalue() const override;

  /// Returns the string representation of the Ir value
  std::string toString() const override;
};

} // namespace SIRF

#endif
