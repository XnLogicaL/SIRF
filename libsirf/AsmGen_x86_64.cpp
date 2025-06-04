// This file is a part of the SIRF (Simple Intermediate Representation Format)
// project Copyright (C) 2025 XnLogical - Licensed under GNU GPL v3.0

#include <Asm/AsmGen.hpp>
#include <IR/IrValueLiteral.hpp>
#include <IR/IrTypeSized.hpp>
#include <cassert>

namespace sirf {

std::string AsmGenerator::generateRegister_x86_64(const IrValueRegister& reg) {
  static std::unordered_map<uint32_t, size_t> regStackOffsetMap;

  if (reg.kind == IrRegisterKind::QWORD) {
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

  if (auto it = regStackOffsetMap.find(reg.id); it != regStackOffsetMap.end())
    return std::format("qword [rbp-{}]", it->second);

  spillOffset += 8;
  regStackOffsetMap[reg.id] = spillOffset;
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

    const int oldrsp = spillOffset;
    const IrStmtFunction* oldFun = currentFunction;

    spillOffset = 0;
    currentFunction = fun;

    for (const IrStmt& bodyStat : fun->body) {
      generateStmt_x86_64(bodyStat);
    }

    spillOffset = oldrsp;
    currentFunction = oldFun;

    section_text << ".L" << fun->id.id << ".epilogue:\n";
    section_text << "  pop rbp\n";
    section_text << "  ret\n";
  }
  else if SIRF_CHECKVIRT (IrStmtInstruction, insn, stat) {
    switch (insn->op) {
    case IrOpCode::RET: {
      if (!insn->ops.empty()) {
        const IrValue& retv = insn->ops.at(0);
        if SIRF_CHECKVIRT (IrValueLiteral, lit, retv) {
          if SIRF_CHECKVIRT (IrTypeSized, sized, lit->type) {
            section_text << "  mov "
                         << "rax, " << lit->value << "\n";
          }
        }
      }

      if (currentFunction) {
        section_text << "  jmp .L" << currentFunction->id.id << ".epilogue\n";
      }
    } break;
    case IrOpCode::MOV: {
      if (insn->ops.size() >= 2) {
        auto a = generateValue_x86_64(insn->ops[0]);
        auto b = generateValue_x86_64(insn->ops[1]);
        section_text << "  mov " << a << ", " << b << "\n";
      }
    } break;
    default:
      break;
    }
  }
  else if SIRF_CHECKVIRT (IrStmtLabel, lb, stat) {
    section_text << lb->label.id << ":\n";
  }
}

std::string AsmGenerator::generateValue_x86_64(const IrValue& val) {
  if SIRF_CHECKVIRT (IrValueLiteral, lit, val)
    return std::to_string(lit->value);
  else if SIRF_CHECKVIRT (IrValueRegister, reg, val)
    return generateRegister_x86_64(*reg);

  assert(false);
  SIRF_UNREACHABLE();
}

} // namespace sirf
