// This file is a part of the SIRF (Simple Intermediate Representation Format)
// project Copyright (C) 2025 XnLogical - Licensed under GNU GPL v3.0

#include "IrAnalyzer.hpp"

namespace sirf {

using enum IrDeclKind;
using enum IrAnalysisResultKind;

template<typename Base, typename T>
  requires std::is_base_of_v<Base, T>
static bool isInstanceOf(const Base* base) {
  return dynamic_cast<const T*>(base) != NULL;
}

static const IrStmtDeclaration* getFuncDecl(const IrHolder& holder, const IrValueSymbol& symbol) {
  for (const IrStmt& stmt : holder.data) {
    const IrStmtDeclaration* stmtDecl = dynamic_cast<const IrStmtDeclaration*>(stmt);
    if (stmtDecl == NULL)
      continue;

    const IrValueSymbol* declSymbol = dynamic_cast<const IrValueSymbol*>(stmtDecl->value);
    if (declSymbol && declSymbol->id == symbol.id)
      return stmtDecl;
  }

  return NULL;
}

static const IrStmtFunction* getFuncDef(const IrHolder& holder, const IrValueSymbol& symbol) {
  for (const IrStmt& stmt : holder.data) {
    const IrStmtFunction* stmtFunDef = dynamic_cast<const IrStmtFunction*>(stmt);
    if (stmtFunDef && stmtFunDef->id.id == symbol.id)
      return stmtFunDef;
  }

  return NULL;
}

const std::vector<IrAnalysisResult> IrAnalyzer::analyzeHolder() const {
  std::vector<IrAnalysisResult> results;

  for (const IrStmt& stmt : holder.data) {
    const auto result = analyzeIrStmt(stmt);
    results.push_back(result);
  }

  return results;
}

const IrAnalysisResult IrAnalyzer::analyzeIrStmt(const IrStmt& stmt) const {
  if (const IrStmtAssign* irAssign = dynamic_cast<const IrStmtAssign*>(stmt))
    return analyzeIrStmtAssign(*irAssign);
  else if (const IrStmtDeclaration* irDecl = dynamic_cast<const IrStmtDeclaration*>(stmt))
    return analyzeIrStmtDeclaration(*irDecl);
  else if (const IrStmtFunction* irFunc = dynamic_cast<const IrStmtFunction*>(stmt))
    return analyzeIrStmtFunction(*irFunc);
  else if (const IrStmtInstruction* irInsn = dynamic_cast<const IrStmtInstruction*>(stmt))
    return analyzeIrStmtInstruction(*irInsn);

  SIRF_UNREACHABLE();
}

const IrAnalysisResult IrAnalyzer::analyzeIrStmtAssign(const IrStmtAssign& irAssign) const {
  if (!irAssign.lvalue->isLvalue())
    return {RES_ERROR, "assignment to non-lvalue"};

  return {OK};
}

const IrAnalysisResult IrAnalyzer::analyzeIrStmtDeclaration(const IrStmtDeclaration& irDecl) const {
  const IrValueSymbol* symbol = dynamic_cast<const IrValueSymbol*>(irDecl.value);
  if (symbol == NULL)
    return {RES_ERROR, "declaration must be a symbol"};

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

    if (!isInstanceOf<IrValueBase, IrValueRegister>(dst))
      return {RES_ERROR, "mov instruction operand 0 must be a register"};

    // clang-format off
    if (!isInstanceOf<IrValueBase, IrValueRegister>(src)
      && !isInstanceOf<IrValueBase, IrValueSSA>(src)
      && !isInstanceOf<IrValueBase, IrValueLiteral>(src)) // clang-format on
      return {RES_ERROR, "mov instruction operand 1 must be a register, literal or SSA"};
  }
  else if (irInsn.op == IrOpCode::LOAD) {
    if (irInsn.ops.size() < 2)
      return {RES_WARN, "load instruction must provide 2 operands"};

    const IrValue& dst = irInsn.ops.at(0);
    const IrValue& src = irInsn.ops.at(1);

    if (!isInstanceOf<IrValueBase, IrValueRegister>(dst))
      return {RES_ERROR, "load instruction operand 0 must be a register"};

    if (!isInstanceOf<IrValueBase, IrValueLabel>(src) && !isInstanceOf<IrValueBase, IrValueSymbol>(src))
      return {RES_ERROR, "load instruction operand 1 must be a pointer"};
  }
  else if (irInsn.op == IrOpCode::STORE) {
    if (irInsn.ops.size() < 2)
      return {RES_WARN, "store instruction must provide 2 operands"};

    const IrValue& dst = irInsn.ops.at(0);
    const IrValue& src = irInsn.ops.at(1);
    // TODO
  }
  else if (irInsn.op == IrOpCode::ADD) {
  }
  else if (irInsn.op == IrOpCode::CALL) {
    if (irInsn.ops.empty())
      return {RES_ERROR, "call instruction must provide operand 0"};

    const IrValue& callee = irInsn.ops.front();
    const IrValueSymbol* symbol = dynamic_cast<const IrValueSymbol*>(callee);
    const IrStmtDeclaration* funDecl = getFuncDecl(holder, *symbol);
    if (funDecl == NULL)
      return {RES_ERROR, "callee symbol not declared"};

    const IrStmtFunction* funDef = getFuncDef(holder, *symbol);
    if (funDecl->kind != EXTERN && funDef == NULL)
      return {RES_ERROR, "undefined reference to symbol"};
  }

  return {OK};
}

} // namespace sirf
