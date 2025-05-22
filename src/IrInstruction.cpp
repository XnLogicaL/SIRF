// This file is a part of the SIRF (Simple Intermediate Representation Format)
// project Copyright (C) 2025 XnLogical - Licensed under GNU GPL v3.0

#include <SIRF/IR/IrInstruction.hpp>

namespace SIRF {

std::string IrInstruction::toString() const {
  std::ostringstream oss;
  oss << SIRF::toString(op) << ' ';

  for (size_t i = 0; const IrValue& op : ops) {
    oss << op->toString();
    if (++i < ops.size()) {
      oss << ", ";
    }
  }

  return oss.str();
}

std::string toString(IrOpCode opcode) {
  using enum IrOpCode;

  // clang-format off
  switch (opcode) {
  case MOV:   return "mov";
  case LOAD:  return "load";
  case STORE: return "store";
  case ADD:   return "add";
  case SUB:   return "sub";
  case MUL:   return "mul";
  case DIV:   return "div";
  case RSP:   return "rsp";
  case SSP:   return "ssp";
  case CALL:  return "call";
  case RET:   return "ret";
  case JMP:   return "jmp";
  case JZ:    return "jz";
  case JNZ:   return "jnz";
  case NOP:
  default:    return "nop";
  } // clang-format on

  SIRF_UNREACHABLE();
}

} // namespace SIRF
