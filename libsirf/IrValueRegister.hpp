// This file is a part of the SIRF (Simple Intermediate Representation Format)
// project Copyright (C) 2025 XnLogical - Licensed under GNU GPL v3.0

#ifndef SIRF_IRVALUEREGISTER_HPP
#define SIRF_IRVALUEREGISTER_HPP

#include "Common.hpp"
#include "IrValueBase.hpp"
#include <arena/arena.h>

namespace sirf {

enum class IrRegisterKind {
  QWORD, // r0
  DWORD, // x0
  WORD,  // w0
  BYTE,  // b0
};

class IrValueRegister final : public IrValueBase {
public:
  const uint32_t id;
  const IrRegisterKind kind;

  explicit IrValueRegister(uint32_t id, IrRegisterKind kind)
    : id(id),
      kind(kind) {}

  static IrValue make(ArenaAllocator& al, uint32_t id, IrRegisterKind kind) {
    return al.emplace<IrValueRegister>(id, kind);
  }

  /// Returns whether if the Ir value can be interpreted as an lvalue
  constexpr bool isLvalue() const override;

  /// Returns the string representation of the Ir value
  std::string toString() const override;
};

} // namespace sirf

#endif
