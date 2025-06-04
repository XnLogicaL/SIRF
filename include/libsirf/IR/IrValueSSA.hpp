// This file is a part of the SIRF (Simple Intermediate Representation Format)
// project Copyright (C) 2025 XnLogical - Licensed under GNU GPL v3.0

#ifndef SIRF_IRVALUESSA_HPP
#define SIRF_IRVALUESSA_HPP

#include <Core/Common.hpp>
#include <IR/IrValueBase.hpp>

namespace sirf {

class IrValueSSA final : public IrValueBase {
public:
  const std::string id;
  const size_t version;

  explicit IrValueSSA(std::string id, size_t version)
    : id(id),
      version(version) {}

  static inline IrValue newValue(std::string id, size_t version) {
    return std::make_shared<IrValueSSA>(id, version);
  }

  /// Returns whether if the Ir value can be interpreted as an lvalue
  constexpr bool isLvalue() const override;

  /// Returns the string representation of the Ir value
  std::string toString() const override;
};

} // namespace sirf

#endif
