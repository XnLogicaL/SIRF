// This file is a part of the SIRF (Simple Intermediate Representation Format)
// project Copyright (C) 2025 XnLogical - Licensed under GNU GPL v3.0

#include <IR/IrAnalyzer.hpp>

namespace SIRF {

using enum IrDeclKind;
using enum IrAnalysisResultKind;

template<typename Base, typename T>
  requires std::is_base_of_v<Base, T>
static bool isInstanceOf(const Base* base) {
  return dynamic_cast<const T*>(base) != nullptr;
}

static const IrStmtDeclaration* getFuncDecl(const IrHolder& holder, const IrValueSymbol& symbol) {
  for (const IrStmt& stmt : holder.get()) {
    const IrStmtDeclaration* stmtDecl = dynamic_cast<const IrStmtDeclaration*>(stmt.get());
    if (stmtDecl == nullptr)
      continue;

    const IrValueSymbol* declSymbol = dynamic_cast<const IrValueSymbol*>(stmtDecl->value.get());
    if (declSymbol && declSymbol->id == symbol.id)
      return stmtDecl;
  }

  return nullptr;
}

static const IrStmtFunction* getFuncDef(const IrHolder& holder, const IrValueSymbol& symbol) {
  for (const IrStmt& stmt : holder.get()) {
    const IrStmtFunction* stmtFunDef = dynamic_cast<const IrStmtFunction*>(stmt.get());
    if (stmtFunDef && stmtFunDef->id.id == symbol.id)
      return stmtFunDef;
  }

  return nullptr;
}

const std::vector<IrAnalysisResult> IrAnalyzer::analyzeHolder() const {
  std::vector<IrAnalysisResult> results;

  for (const IrStmt& stmt : holder.get()) {
    const auto result = analyzeIrStmt(stmt);
    results.push_back(result);
  }

  return results;
}

const IrAnalysisResult IrAnalyzer::analyzeIrStmt(const IrStmt& stmt) const {
  if (const IrStmtAssign* irAssign = dynamic_cast<const IrStmtAssign*>(stmt.get()))
    return analyzeIrStmtAssign(*irAssign);
  else if (const IrStmtDeclaration* irDecl = dynamic_cast<const IrStmtDeclaration*>(stmt.get()))
    return analyzeIrStmtDeclaration(*irDecl);
  else if (const IrStmtFunction* irFunc = dynamic_cast<const IrStmtFunction*>(stmt.get()))
    return analyzeIrStmtFunction(*irFunc);
  else if (const IrStmtInstruction* irInsn = dynamic_cast<const IrStmtInstruction*>(stmt.get()))
    return analyzeIrStmtInstruction(*irInsn);

  SIRF_UNREACHABLE();
}

const IrAnalysisResult IrAnalyzer::analyzeIrStmtAssign(const IrStmtAssign& irAssign) const {
  if (!irAssign.lvalue->isLvalue())
    return {RES_ERROR, "Assignment to non-lvalue"};

  return {OK};
}

const IrAnalysisResult IrAnalyzer::analyzeIrStmtDeclaration(const IrStmtDeclaration& irDecl) const {
  const IrValueSymbol* symbol = dynamic_cast<const IrValueSymbol*>(irDecl.value.get());
  if (symbol == nullptr)
    return {RES_ERROR, "Declaration must be a symbol"};

  return {OK};
}

const IrAnalysisResult IrAnalyzer::analyzeIrStmtFunction(const IrStmtFunction& irFunc) const {
  return {OK};
}

const IrAnalysisResult IrAnalyzer::analyzeIrStmtInstruction(const IrStmtInstruction& irInsn) const {
  if (irInsn.op == IrOpCode::NOP) {
    if (!irInsn.ops.empty())
      return {RES_WARN, "nop instruction must provide no operands"};
  }
  else if (irInsn.op == IrOpCode::MOV) {
    if (irInsn.ops.size() < 2)
      return {RES_WARN, "mov instruction must provide 2 operands"};

    const IrValue& dst = irInsn.ops.at(0);
    const IrValue& src = irInsn.ops.at(1);

    if (!isInstanceOf<IrValueBase, IrValueRegister>(dst.get()))
      return {RES_ERROR, "mov instruction operand 0 must be a register"};

    // clang-format off
    if (!isInstanceOf<IrValueBase, IrValueRegister>(src.get())
      && !isInstanceOf<IrValueBase, IrValueSSA>(src.get())
      && !isInstanceOf<IrValueBase, IrValueLiteral>(src.get())) // clang-format on
      return {RES_ERROR, "mov instruction operand 1 must be a register, literal or SSA"};
  }
  else if (irInsn.op == IrOpCode::CALL) {
    if (irInsn.ops.empty())
      return {RES_ERROR, "call instruction must provide operand 0"};

    const IrValue& callee = irInsn.ops.front();
    const IrValueSymbol* symbol = dynamic_cast<const IrValueSymbol*>(callee.get());
    const IrStmtDeclaration* funDecl = getFuncDecl(holder, *symbol);
    if (funDecl == nullptr)
      return {RES_ERROR, "Callee symbol not declared"};

    const IrStmtFunction* funDef = getFuncDef(holder, *symbol);
    if (funDecl->kind != EXTERN && funDef == nullptr)
      return {RES_ERROR, "Undefined reference to symbol"};
  }

  return {OK};
}

} // namespace SIRF
