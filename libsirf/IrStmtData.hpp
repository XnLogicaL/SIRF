// This file is a part of the SIRF (Simple Intermediate Representation Format)
// project Copyright (C) 2025 XnLogical - Licensed under GNU GPL v3.0

#ifndef SIRF_IRSTMTDATA_HPP
#define SIRF_IRSTMTDATA_HPP

#include "Common.hpp"
#include "IrStmtBase.hpp"
#include "IrValueLabel.hpp"
#include <arena/arena.h>

namespace sirf {

// TODO: Use HeapBuffer when implemented.
using DataBuf = std::vector<uint8_t>;

class IrStmtData final : public IrStmtBase {
public:
  const IrValueLabel id;
  const DataBuf buf;

  explicit IrStmtData(IrValueLabel&& id, DataBuf&& buf)
    : id(std::move(id)),
      buf(std::move(buf)) {}

  inline static IrStmt make(ArenaAllocator& al, IrValueLabel&& id, DataBuf&& buf) {
    return al.emplace<IrStmtData>(std::move(id), std::move(buf));
  }

  std::string toString() const override;
};

} // namespace sirf

#endif
