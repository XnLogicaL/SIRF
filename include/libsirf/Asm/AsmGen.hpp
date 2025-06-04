// This file is a part of the SIRF (Simple Intermediate Representation Format)
// project Copyright (C) 2025 XnLogical - Licensed under GNU GPL v3.0

#ifndef SIRF_ASMGENBASE_HPP
#define SIRF_ASMGENBASE_HPP

#include <Core/Common.hpp>
#include <IR/IrHolder.hpp>
#include <IR/IrStmtAssign.hpp>
#include <IR/IrStmtDeclaration.hpp>
#include <IR/IrStmtFunction.hpp>
#include <IR/IrStmtInstruction.hpp>
#include <IR/IrStmtLabel.hpp>

namespace sirf {

class AsmGenerator {
public:
  explicit AsmGenerator(const IrHolder& holder)
    : holder(holder) {}

  std::string generate();

private:
  void generateTarget_x86_64(const IrStmt& stat);

private:
  const IrHolder& holder;

  const IrStmtFunction* currentFunction = NULL;

  std::ostringstream section_data;
  std::ostringstream section_text;
  std::ostringstream section_bss;
};

} // namespace sirf

#endif
