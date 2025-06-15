// This file is a part of the SIRF (Simple Intermediate Representation Format)
// project Copyright (C) 2025 XnLogical - Licensed under GNU GPL v3.0

#ifndef SIRF_IRTYPESIZED_HPP
#define SIRF_IRTYPESIZED_HPP

#include "Common.hpp"
#include "IrTypeBase.hpp"
#include <arena/arena.h>

namespace sirf {

class IrTypeSized final : public IrTypeBase {
public:
  const bool isSigned;
  const uint64_t size;

  explicit IrTypeSized(bool isSigned, uint64_t size)
    : isSigned(isSigned),
      size(size) {}

  static inline IrType make(ArenaAllocator& al, bool isSigned, uint64_t size) {
    return al.emplace<IrTypeSized>(isSigned, size);
  }

  std::string toString() const override;
};

} // namespace sirf

#endif
