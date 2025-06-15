// This file is a part of the SIRF (Simple Intermediate Representation Format)
// project Copyright (C) 2025 XnLogical - Licensed under GNU GPL v3.0

#ifndef SIRF_IRSTMTASSIGN_HPP
#define SIRF_IRSTMTASSIGN_HPP

#include "Common.hpp"
#include "IrValueBase.hpp"
#include "IrValueSSA.hpp"
#include "IrStmtBase.hpp"
#include <arena/arena.h>

namespace sirf {

class IrStmtAssign final : public IrStmtBase {
public:
  const IrValue lvalue;
  const IrValue rvalue;

  explicit IrStmtAssign(IrValue lvalue, IrValue rvalue)
    : lvalue(lvalue),
      rvalue(rvalue) {}

  inline static IrStmt make(ArenaAllocator& al, IrValue lvalue, IrValue rvalue) {
    return al.emplace<IrStmtAssign>(std::move(lvalue), std::move(rvalue));
  }

  std::string toString() const override;
};

} // namespace sirf

#endif
