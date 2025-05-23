// This file is a part of the SIRF (Simple Intermediate Representation Format)
// project Copyright (C) 2025 XnLogical - Licensed under GNU GPL v3.0

#include <IR/IrAnalyzer.hpp>

namespace SIRF {

using enum IrAnalysisResultKind;

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
  if (!irAssign.lvalue->isLvalue()) {
    return {RES_ERROR, "Assignment to non-lvalue"};
  }

  return {OK};
}

const IrAnalysisResult IrAnalyzer::analyzeIrStmtDeclaration(const IrStmtDeclaration& irDecl) const {
  const IrValueSymbol* symbol = dynamic_cast<const IrValueSymbol*>(irDecl.value.get());
  if (symbol == nullptr) {
    return {RES_ERROR, "Declaration must be a symbol"};
  }

  return {OK};
}

const IrAnalysisResult IrAnalyzer::analyzeIrStmtFunction(const IrStmtFunction& irFunc) const {
  return {OK};
}

const IrAnalysisResult IrAnalyzer::analyzeIrStmtInstruction(const IrStmtInstruction& irFunc) const {
  return {OK};
}

} // namespace SIRF
