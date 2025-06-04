// This file is a part of the SIRF (Simple Intermediate Representation Format)
// project Copyright (C) 2025 XnLogical - Licensed under GNU GPL v3.0

#ifndef SIRF_IRANALYZER_HPP
#define SIRF_IRANALYZER_HPP

#include <Core/Common.hpp>
#include <IR/IrValues.hpp>
#include <IR/IrTypes.hpp>
#include <IR/IrStmts.hpp>
#include <IR/IrHolder.hpp>

namespace sirf {

enum class IrAnalysisResultKind {
  OK,
  RES_INFO,
  RES_WARN,
  RES_ERROR,
};

struct IrAnalysisResult {
  const IrAnalysisResultKind kind;
  const std::string message;
};

class IrAnalyzer final {
public:
  explicit IrAnalyzer(const IrHolder& holder)
    : holder(holder) {}

  /// Analyze singular IR statement and return the result
  const IrAnalysisResult analyzeIrStmt(const IrStmt& stmt) const;

  /// Analyze IrAnalyzer::holder and return all analysis results
  const std::vector<IrAnalysisResult> analyzeHolder() const;

private:
  const IrAnalysisResult analyzeIrStmtAssign(const IrStmtAssign& assign) const;
  const IrAnalysisResult analyzeIrStmtFunction(const IrStmtFunction& func) const;
  const IrAnalysisResult analyzeIrStmtDeclaration(const IrStmtDeclaration& decl) const;
  const IrAnalysisResult analyzeIrStmtInstruction(const IrStmtInstruction& insn) const;

private:
  const IrHolder& holder;
};

} // namespace sirf

#endif
