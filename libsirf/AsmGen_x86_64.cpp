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

static std::string getEpilogueLabelName(const std::string& func_name) {
  return std::format(".L{}.epilogue", func_name);
}

static std::string getStackOffsetPtr(size_t offset) {
  return std::format("[rbp-{}]", offset);
}

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

void AsmGenerator::generateBinaryInstruction_x86_64(const std::string& opName, const ValueRepr& dst, const ValueRepr& src) {
  std::string dst_val = dst.val, src_val = src.val;

  if (dst.isPointer) {
    if (src.isPointer) {
      section_text << "  mov r11, " << src.val << "\n";
      dst_val = "r11";
    }
    else {
      const int size = getSizeOf(dst.og_val);
      const std::string pref = getSizePrefix(size);
      dst_val = pref + ' ' + dst.val;
    }
  }

  section_text << "  " << opName << ' ' << dst_val << ", " << src_val << "\n";
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
      return {false, it->second, &reg};
  }

  auto& currentMap = stackMap.back();
  if (auto it = currentMap.find(reg.id); it != currentMap.end() && it->second.kind == spill)
    return {true, getStackOffsetPtr(it->second.u.reg.offset), &reg};

  stackOffset += 8;
  currentMap[reg.id] = {spill, {.reg = {stackOffset}}};
  section_text << "  sub rsp, 8\n";
  return {true, getStackOffsetPtr(stackOffset), &reg};
}

ValueRepr AsmGenerator::generateVariable_x86_64(const IrValueSSA& var) {
  auto& currentMap = stackMap.back();
  if (auto it = currentMap.find(var.id); it != currentMap.end())
    return {true, getStackOffsetPtr(it->second.u.var.offset), &var};

  // variable must be allocated via `alloca`
  SIRF_TODO();
  SIRF_UNREACHABLE();
}

void AsmGenerator::generateStmt_x86_64(const IrStmt& stat) {
  if SIRF_CHECKVIRT (IrStmtDeclaration, decl, stat.get()) {
    if SIRF_CHECKVIRT (IrValueSymbol, sym, decl->value.get()) {
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
  else if SIRF_CHECKVIRT (IrStmtFunction, fun, stat.get()) {
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

    section_text << getEpilogueLabelName(fun->id.id) << ":\n";
    section_text << "  leave\n";
    section_text << "  ret\n";
  }
  else if SIRF_CHECKVIRT (IrStmtInstruction, insn, stat.get()) {
    using enum IrOpCode;

    switch (insn->op) {
    case RET: {
      if (!insn->ops.empty()) {
        const IrValue& retv = insn->ops[0];
        const ValueRepr src = generateValue_x86_64(retv);

        // check if `src` is the return register, in which case we dont have to mov it
        if (src.val != "rax")
          section_text << "  mov rax, " << src.val << "\n";
      }

      if (currentFunction)
        section_text << "  jmp " << getEpilogueLabelName(currentFunction->id.id) << "\n";
    } break;
    case ALLOCA: {
      const IrValue& dst = insn->ops[0];
      const IrValue& size = insn->ops[1];

      if SIRF_CHECKVIRT (IrValueSSA, ssa, dst.get()) {
        auto& currentMap = stackMap.back();

        if SIRF_CHECKVIRT (IrValueLiteral, lit, size.get()) {
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
        if SIRF_CHECKVIRT (IrValueLiteral, lit, insn->ops[1].get()) {
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
      generateBinaryInstruction_x86_64(opName, dst, src);
    } break;
    default:
      break;
    }
  }
  else if SIRF_CHECKVIRT (IrStmtLabel, lb, stat.get())
    section_text << lb->label.id << ":\n";
  else if SIRF_CHECKVIRT (IrStmtAssign, as, stat.get()) {
    ValueRepr lval = generateValue_x86_64(as->lvalue);
    ValueRepr rval = generateValue_x86_64(as->rvalue);
    generateBinaryInstruction_x86_64("mov", lval, rval);
  }
  else {
    SIRF_TODO();
    SIRF_UNREACHABLE();
  }
}

ValueRepr AsmGenerator::generateValue_x86_64(const IrValue& val) {
  if SIRF_CHECKVIRT (IrValueLiteral, lit, val.get())
    return {false, std::to_string(lit->value), val.get()};
  else if SIRF_CHECKVIRT (IrValueRegister, reg, val.get())
    return generateRegister_x86_64(*reg);
  else if SIRF_CHECKVIRT (IrValueSSA, ssa, val.get())
    return generateVariable_x86_64(*ssa);

  SIRF_TODO();
  SIRF_UNREACHABLE();
}

} // namespace sirf
