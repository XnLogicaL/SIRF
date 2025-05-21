// This file is a part of the SIRF (Simple Intermediate Representation Format)
// project Copyright (C) 2025 XnLogical - Licensed under GNU GPL v3.0

#ifndef SIRF_IRINSTRUCTION_HPP
#define SIRF_IRINSTRUCTION_HPP

#include <cstdint>
#include <vector>
#include <string>
#include <sstream>
#include <memory>

#include <SIRF/Core/APIConfig.hpp>
#include <SIRF/Core/APIMacros.hpp>
#include <SIRF/IR/IrValueBase.hpp>

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
};

using IrOperand = std::unique_ptr<IrValueBase>;
using IrOperands = std::vector<IrOperand>;

class IrInstruction {
public:
  const IrOpCode op;
  const IrOperands ops;

  explicit IrInstruction(IrOpCode op, IrOperands ops)
    : op(op),
      ops(std::move(ops)) {}

  std::string toString() const;
};

std::string toString(IrOpCode opcode);

} // namespace SIRF

#endif
