// This file is a part of the SIRF (Simple Intermediate Representation Format)
// project Copyright (C) 2025 XnLogical - Licensed under GNU GPL v3.0

#ifndef SIRF_IRTYPENAMED_HPP
#define SIRF_IRTYPENAMED_HPP

#include "Common.hpp"
#include "IrTypeBase.hpp"
#include <arena/arena.h>

namespace sirf {

enum class IrNamedTypeKind {
  VOID,
  BOOL,
  BYTE,
  WORD,
  DWORD,
  QWORD,
};

class IrTypeNamed final : public IrTypeBase {
public:
  const IrNamedTypeKind kind;

  explicit IrTypeNamed(IrNamedTypeKind kind)
    : kind(kind) {}

  static inline IrType make(ArenaAllocator& al, IrNamedTypeKind kind) {
    return al.emplace<IrTypeNamed>(kind);
  }

  std::string toString() const override;
};

std::string toString(IrNamedTypeKind kind);

} // namespace sirf

#endif
