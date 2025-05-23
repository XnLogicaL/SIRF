// This file is a part of the SIRF (Simple Intermediate Representation Format)
// project Copyright (C) 2025 XnLogical - Licensed under GNU GPL v3.0

#ifndef SIRF_IRTYPEBITFIELD_HPP
#define SIRF_IRTYPEBITFIELD_HPP

#include <Core/Common.hpp>
#include <IR/IrTypeBase.hpp>

namespace SIRF {

class IrTypeBitfield final : public IrTypeBase {
public:
  const bool isSigned;
  const uint64_t size;

  explicit IrTypeBitfield(bool isSigned, uint64_t size)
    : isSigned(isSigned),
      size(size) {}
};

} // namespace SIRF

#endif
