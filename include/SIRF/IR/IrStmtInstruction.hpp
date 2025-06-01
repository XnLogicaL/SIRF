// This file is a part of the SIRF (Simple Intermediate Representation Format)
// project Copyright (C) 2025 XnLogical - Licensed under GNU GPL v3.0

#ifndef SIRF_IRSTMTINSTRUCTION_HPP
#define SIRF_IRSTMTINSTRUCTION_HPP

#include <Core/Common.hpp>
#include <IR/IrValueBase.hpp>
#include <IR/IrStmtBase.hpp>

namespace SIRF {

enum class IrOpCode {
  NOP,
  MOV,
  LOAD,
  STORE,
  ADD,
  SUB,
  MUL,
  DIV,
  RSP,
  SSP,
  CALL,
  RET,
  JMP,
  JZ,
  JNZ,
};

using IrOperands = std::vector<IrValue>;

class IrStmtInstruction final : public IrStmtBase {
public:
  const IrOpCode op;
  const IrOperands ops;

  explicit IrStmtInstruction(IrOpCode op, IrOperands ops)
    : op(op),
      ops(std::move(ops)) {}

  static inline IrStmt newStmt(IrOpCode op, IrOperands&& ops) {
    return std::make_shared<IrStmtInstruction>(op, std::move(ops));
  }

  std::string toString() const override;
};

std::string toString(IrOpCode opcode);

} // namespace SIRF

#endif
