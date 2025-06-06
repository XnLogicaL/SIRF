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

std::string AsmGenerator::generateRegister_x86_64(const IrValueRegister& reg) {
  if (reg.kind == QWORD) {
    static std::unordered_map<uint32_t, const char*> regMap = {
      {0, "rax"},
      {1, "rbx"},
      {2, "rcx"},
      {3, "rdx"},
      {4, "rsi"},
      {5, "rdi"},
      {6, "r8"},
      {7, "r9"},
      {8, "r10"},
      {9, "r11"},
    };

    if (auto it = regMap.find(reg.id); it != regMap.end())
      return it->second;
  }

  auto& currentMap = stackMap.back();
  if (auto it = currentMap.find(reg.id); it != currentMap.end() && it->second.kind == registerSpill)
    return std::format("qword [rbp-{}]", it->second.u.reg.offset);

  spillOffset += 8;
  currentMap[reg.id] = {registerSpill, {.reg = {spillOffset}}};
  section_text << "  sub rsp, 8                    ; r" << reg.id << " spill\n";
  return std::format("qword [rbp-{}]", spillOffset);
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

    const size_t oldSpillOffset = spillOffset;
    const IrStmtFunction* oldFun = currentFunction;

    spillOffset = 0;
    currentFunction = fun;
    stackMap.push_back({});

    for (const IrStmt& bodyStat : fun->body) {
      generateStmt_x86_64(bodyStat);
    }

    spillOffset = oldSpillOffset;
    currentFunction = oldFun;
    stackMap.pop_back();

    section_text << ".L" << fun->id.id << ".epilogue:\n";
    section_text << "  pop rbp\n";
    section_text << "  ret\n";
  }
  else if SIRF_CHECKVIRT (IrStmtInstruction, insn, stat) {
    using enum IrOpCode;

    switch (insn->op) {
    case RET: {
      if (!insn->ops.empty()) {
        const IrValue& retv = insn->ops.at(0);
        if SIRF_CHECKVIRT (IrValueLiteral, lit, retv) {
          if SIRF_CHECKVIRT (IrTypeSized, sized, lit->type) {
            section_text << "  mov rax, " << lit->value << "\n";
          }
        }
      }

      if (currentFunction) {
        section_text << "  jmp .L" << currentFunction->id.id << ".epilogue\n";
      }
    } break;
    case ADD:
    case SUB:
    case MUL:
    case DIV:
    case MOV: {
      auto a = generateValue_x86_64(insn->ops[0]);
      auto opName = std::string(magic_enum::enum_name(insn->op));
      std::transform(opName.begin(), opName.end(), opName.begin(), ::tolower);

      // attempt mul/div strength reduction
      if (insn->op == MUL || insn->op == DIV) {
        if SIRF_CHECKVIRT (IrValueLiteral, lit, insn->ops[1]) {
          if (lit->value > 0 && (lit->value & (lit->value - 1)) == 0) {
            int shift = static_cast<int>(std::log2(lit->value));
            section_text << (insn->op == MUL ? "  shl " : "  shr ") << a << ", " << shift << "\n";
            break;
          }
        }
      }

      auto b = generateValue_x86_64(insn->ops[1]);
      section_text << "  " << opName << ' ' << a << ", " << b << "\n";
    } break;
    default:
      break;
    }
  }
  else if SIRF_CHECKVIRT (IrStmtLabel, lb, stat) {
    section_text << lb->label.id << ":\n";
  }

  SIRF_TODO();
  SIRF_UNREACHABLE();
}

std::string AsmGenerator::generateValue_x86_64(const IrValue& val) {
  if SIRF_CHECKVIRT (IrValueLiteral, lit, val)
    return std::to_string(lit->value);
  else if SIRF_CHECKVIRT (IrValueRegister, reg, val)
    return generateRegister_x86_64(*reg);

  SIRF_TODO();
  SIRF_UNREACHABLE();
}

} // namespace sirf
