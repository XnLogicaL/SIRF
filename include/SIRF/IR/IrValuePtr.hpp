// This file is a part of the SIRF (Simple Intermediate Representation Format)
// project Copyright (C) 2025 XnLogical - Licensed under GNU GPL v3.0

#ifndef SIRF_IRVALUEPTR_HPP
#define SIRF_IRVALUEPTR_HPP

#include <Core/Common.hpp>
#include <IR/IrValueBase.hpp>

namespace SIRF {

class IrValuePtr final : public IrValueBase {
public:
  const IrValue val;

  explicit IrValuePtr(IrValue val)
    : val(val) {}

  constexpr bool isLvalue() const override;

  std::string toString() const override;
};

} // namespace SIRF

#endif
