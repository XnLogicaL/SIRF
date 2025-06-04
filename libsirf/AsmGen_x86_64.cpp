// This file is a part of the SIRF (Simple Intermediate Representation Format)
// project Copyright (C) 2025 XnLogical - Licensed under GNU GPL v3.0

#include <Asm/AsmGen.hpp>
#include <IR/IrValueLiteral.hpp>
#include <IR/IrTypeSized.hpp>

namespace sirf {

#define CHECK_VIRTUAL(type, name, target) (const type* name = dynamic_cast<const type*>(target.get()))

static char getRegisterPrefixBySize(uint64_t size) {
  // clang-format off
  switch (size) {
  case 32: return 'e';
  case 64: return 'r';
  default:
    break;
  } // clang-format on

  return 0x00;
}

void AsmGenerator::generateTarget_x86_64(const IrStmt& stat) {
  if CHECK_VIRTUAL (IrStmtDeclaration, decl, stat) {
    if CHECK_VIRTUAL (IrValueSymbol, sym, decl->value) {
      switch (decl->kind) {
      case IrDeclKind::EXTERN:
        section_text << "extern " << sym->id << "\n";
        break;
      case IrDeclKind::GLOBAL:
        section_text << "global " << sym->id << "\n";
      default:
        // TODO: Add diagnostic
        break;
      }
    }
  }
  else if CHECK_VIRTUAL (IrStmtFunction, fun, stat) {
    section_text << fun->id.id << ":\n";
    section_text << "  push rbp\n";
    section_text << "  mov rbp, rsp\n";

    const IrStmtFunction* oldFun = currentFunction;
    currentFunction = fun;

    for (const IrStmt& bodyStat : fun->body) {
      generateTarget_x86_64(bodyStat);
    }

    currentFunction = oldFun;

    section_text << ".L" << fun->id.id << ".epilogue:\n";
    section_text << "  pop rbp\n";
    section_text << "  ret\n";
  }
  else if CHECK_VIRTUAL (IrStmtInstruction, insn, stat) {
    switch (insn->op) {
    case IrOpCode::RET: {
      if (!insn->ops.empty()) {
        const IrValue& retv = insn->ops.at(0);
        if CHECK_VIRTUAL (IrValueLiteral, lit, retv) {
          if CHECK_VIRTUAL (IrTypeSized, sized, lit->type) {
            char regPref = getRegisterPrefixBySize(sized->size);
            if (regPref == 0x00) {
              // TODO: Add diagnostic
              return;
            }

            section_text << "  mov " << regPref << "ax, " << lit->value << "\n";
          }
          else {
            // TODO: Add diagnostic
          }
        }
        else {
          // TODO: Add diagnostic
        }
      }

      if (currentFunction) {
        section_text << "  jmp .L" << currentFunction->id.id << ".epilogue\n";
      }
      else {
        // TODO: Add diagnostic
      }
    }
    default:
      // TODO: Add diagnostics
      break;
    }
  }
  else if CHECK_VIRTUAL (IrStmtLabel, lb, stat) {
    section_text << lb->toString();
  }
}

} // namespace sirf
