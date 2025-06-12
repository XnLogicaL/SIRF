// This file is a part of the SIRF (Simple Intermediate Representation Format)
// project Copyright (C) 2025 XnLogical - Licensed under GNU GPL v3.0

#include <Asm/AsmGen.hpp>
#include <IR/IrValueLiteral.hpp>
#include <IR/IrTypeSized.hpp>
#include <cassert>
#include <cmath>

namespace sirf {

using enum StkIdKind;
using enum IrRegisterKind;

size_t AsmGenerator::alloca_x86_64(size_t size) {
  // Round size up to nearest multiple of 16
  size_t aligned_size = ((size + 15) / 16) * 16;

  if (allocaRemBytes < size) {
    stackOffset += aligned_size;
    allocaRemBytes = aligned_size;
    section_text << "  sub rsp, " << aligned_size << "\n";
  }

  allocaRemBytes -= size;
  return stackOffset - allocaRemBytes - size;
}

ValueRepr AsmGenerator::generateRegister_x86_64(const IrValueRegister& reg) {
  if (reg.kind == QWORD) {
    static std::unordered_map<uint32_t, const char*> regMap = {
      {0, "rax"},
      {1, "rcx"},
      {2, "rdx"},
      {3, "rsi"},
      {4, "rdi"},
      {5, "r8"},
      {6, "r9"},
      {7, "r10"},
    };

    if (auto it = regMap.find(reg.id); it != regMap.end())
      return {it->second, false};
  }

  auto& currentMap = stackMap.back();
  if (auto it = currentMap.find(reg.id); it != currentMap.end() && it->second.kind == spill)
    return {std::format("[rbp-{}]", it->second.u.reg.offset), true};

  stackOffset += 8;
  currentMap[reg.id] = {spill, {.reg = {stackOffset}}};
  section_text << "  sub rsp, 8     ; r" << reg.id << " spill\n";
  return {std::format("[rbp-{}]", stackOffset), true};
}

ValueRepr AsmGenerator::generateVariable_x86_64(const IrValueSSA& var) {
  auto& currentMap = stackMap.back();
  if (auto it = currentMap.find(var.id); it != currentMap.end())
    return {std::format("[rbp-{}]", it->second.u.var.offset), true};

  // variable must be allocated via `alloca`
  SIRF_TODO();
  SIRF_UNREACHABLE();
}

void AsmGenerator::generateStmt_x86_64(const IrStmt& stat) {
  if SIRF_CHECKVIRT (IrStmtDeclaration, decl, stat) {
    if SIRF_CHECKVIRT (IrValueSymbol, sym, decl->value) {
      switch (decl->kind) {
      case IrDeclKind::EXTERN:
        section_text << "extern " << sym->id << "\n";
        break;
      case IrDeclKind::GLOBAL:
        section_text << "global " << sym->id << "\n";
      default:
        break;
      }
    }
  }
  else if SIRF_CHECKVIRT (IrStmtFunction, fun, stat) {
    section_text << fun->id.id << ":\n";
    section_text << "  push rbp\n";
    section_text << "  mov rbp, rsp\n";

    const size_t oldstackOffset = stackOffset;
    const IrStmtFunction* oldFun = currentFunction;

    stackOffset = 0;
    currentFunction = fun;
    stackMap.push_back({});

    for (const IrStmt& bodyStat : fun->body) {
      generateStmt_x86_64(bodyStat);
    }

    stackOffset = oldstackOffset;
    currentFunction = oldFun;
    stackMap.pop_back();

    section_text << ".L" << fun->id.id << ".epilogue:\n";
    section_text << "  leave\n";
    section_text << "  ret\n";
  }
  else if SIRF_CHECKVIRT (IrStmtInstruction, insn, stat) {
    using enum IrOpCode;

    switch (insn->op) {
    case RET: {
      if (!insn->ops.empty()) {
        const IrValue& retv = insn->ops[0];
        const ValueRepr src = generateValue_x86_64(retv);
        section_text << "  mov rax, " << src.val << "\n";
      }

      if (currentFunction)
        section_text << "  jmp .L" << currentFunction->id.id << ".epilogue\n";
    } break;
    case ALLOCA: {
      const IrValue& dst = insn->ops[0];
      const IrValue& size = insn->ops[1];

      if SIRF_CHECKVIRT (IrValueSSA, ssa, dst) {
        auto& currentMap = stackMap.back();

        if SIRF_CHECKVIRT (IrValueLiteral, lit, size) {
          const size_t size = lit->value;
          const size_t offset = alloca_x86_64(size);
          currentMap[ssa->id] = {variable, {.var = {offset, size}}};
        }
      }
    } break;
    case ADD:
    case SUB:
    case MUL:
    case DIV:
    case MOV: {
      ValueRepr dst = generateValue_x86_64(insn->ops[0]);

      // attempt mul/div strength reduction
      if (insn->op == MUL || insn->op == DIV) {
        if SIRF_CHECKVIRT (IrValueLiteral, lit, insn->ops[1]) {
          if (lit->value > 0 && (lit->value & (lit->value - 1)) == 0) {
            int shift = static_cast<int>(std::log2(lit->value));
            section_text << (insn->op == MUL ? "  shl " : "  shr ") << dst.val << ", " << shift << "\n";
            break;
          }
        }
      }

      std::string opName(magic_enum::enum_name(insn->op));
      // lower op name, technically not needed, but we do it anyways for convention
      std::transform(opName.begin(), opName.end(), opName.begin(), ::tolower);

      ValueRepr src = generateValue_x86_64(insn->ops[1]);
      if (dst.isPointer) {
        const int size = getSizeOf(insn->ops[0]);
        std::string spec = getSizePrefix(size);
        dst.val = spec + ' ' + dst.val;

        if (src.isPointer) {
          section_text << "  mov r11, " << src.val << "\n";
          src.val = "r11";
        }
      }

      section_text << "  " << opName << ' ' << dst.val << ", " << src.val << "\n";
    } break;
    default:
      break;
    }
  }
  else if SIRF_CHECKVIRT (IrStmtLabel, lb, stat)
    section_text << lb->label.id << ":\n";
  else if SIRF_CHECKVIRT (IrStmtAssign, as, stat) {
    ValueRepr lval = generateValue_x86_64(as->lvalue);
    ValueRepr rval = generateValue_x86_64(as->rvalue);

    if (lval.isPointer) {
      const int size = getSizeOf(as->lvalue);
      std::string spec = getSizePrefix(size);
      lval.val = spec + ' ' + lval.val;

      if (rval.isPointer) {
        section_text << "  mov r11, " << rval.val << "\n";
        rval.val = "r11";
      }
    }

    section_text << "  mov " << lval.val << ", " << rval.val << "\n";
  }
  else {
    SIRF_TODO();
    SIRF_UNREACHABLE();
  }
}

ValueRepr AsmGenerator::generateValue_x86_64(const IrValue& val) {
  if SIRF_CHECKVIRT (IrValueLiteral, lit, val)
    return {std::to_string(lit->value), false};
  else if SIRF_CHECKVIRT (IrValueRegister, reg, val)
    return generateRegister_x86_64(*reg);
  else if SIRF_CHECKVIRT (IrValueSSA, ssa, val)
    return generateVariable_x86_64(*ssa);

  SIRF_TODO();
  SIRF_UNREACHABLE();
}

} // namespace sirf
