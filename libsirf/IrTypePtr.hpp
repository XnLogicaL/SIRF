// This file is a part of the SIRF (Simple Intermediate Representation Format)
// project Copyright (C) 2025 XnLogical - Licensed under GNU GPL v3.0

#ifndef SIRF_IRTYPEPTR_HPP
#define SIRF_IRTYPEPTR_HPP

#include "Common.hpp"
#include "IrTypeBase.hpp"
#include <arena/arena.h>

namespace sirf {

class IrTypePtr final : public IrTypeBase {
public:
  const IrType type;

  explicit IrTypePtr(IrType&& type)
    : type(std::move(type)) {}

  static inline IrType make(ArenaAllocator& al, IrType&& type) {
    return al.emplace<IrTypePtr>(std::move(type));
  }

  std::string toString() const override;
};

} // namespace sirf

#endif
