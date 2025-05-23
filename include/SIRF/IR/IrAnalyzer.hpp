// This file is a part of the SIRF (Simple Intermediate Representation Format)
// project Copyright (C) 2025 XnLogical - Licensed under GNU GPL v3.0

#ifndef SIRF_IRANALYZER_HPP
#define SIRF_IRANALYZER_HPP

#include <Core/Common.hpp>
#include <IR/IrStmtAssign.hpp>
#include <IR/IrStmtDeclaration.hpp>
#include <IR/IrStmtFunction.hpp>
#include <IR/IrStmtInstruction.hpp>
#include <IR/IrValueRegister.hpp>
#include <IR/IrValueSSA.hpp>
#include <IR/IrValueLiteral.hpp>
#include <IR/IrValueSymbol.hpp>
#include <IR/IrValueLabel.hpp>
#include <IR/IrHolder.hpp>

namespace SIRF {

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

} // namespace SIRF

#endif
