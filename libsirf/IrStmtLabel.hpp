// This file is a part of the SIRF (Simple Intermediate Representation Format)
// project Copyright (C) 2025 XnLogical - Licensed under GNU GPL v3.0

#ifndef SIRF_IRSTMTLABEL_HPP
#define SIRF_IRSTMTLABEL_HPP

#include "Common.hpp"
#include "IrValueLabel.hpp"
#include "IrStmtBase.hpp"
#include <arena/arena.h>

namespace sirf {

class IrStmtLabel final : public IrStmtBase {
public:
  const IrValueLabel label;

  explicit IrStmtLabel(IrValueLabel&& label)
    : label(label) {}

  static inline IrStmt newStmt(ArenaAllocator& al, IrValueLabel&& label) {
    return al.emplace<IrStmtLabel>(std::move(label));
  }

  std::string toString() const override;
};

} // namespace sirf

#endif
