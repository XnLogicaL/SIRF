// This file is a part of the SIRF (Simple Intermediate Representation Format)
// project Copyright (C) 2025 XnLogical - Licensed under GNU GPL v3.0

#ifndef SIRF_IRTYPESIZED_HPP
#define SIRF_IRTYPESIZED_HPP

#include <Core/Common.hpp>
#include <IR/IrTypeBase.hpp>

namespace SIRF {

class IrTypeSized final : public IrTypeBase {
public:
  const bool isSigned;
  const uint64_t size;

  explicit IrTypeSized(bool isSigned, uint64_t size)
    : isSigned(isSigned),
      size(size) {}

  static inline IrType newType(bool isSigned, uint64_t size) {
    return std::make_shared<IrTypeSized>(isSigned, size);
  }

  std::string toString() const override;
};

} // namespace SIRF

#endif
