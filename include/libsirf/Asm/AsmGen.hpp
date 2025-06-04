// This file is a part of the SIRF (Simple Intermediate Representation Format)
// project Copyright (C) 2025 XnLogical - Licensed under GNU GPL v3.0

#ifndef SIRF_ASMGENBASE_HPP
#define SIRF_ASMGENBASE_HPP

#include <Core/Common.hpp>
#include <IR/IrHolder.hpp>
#include <IR/IrValues.hpp>
#include <IR/IrTypes.hpp>
#include <IR/IrStmts.hpp>

#define SIRF_CHECKVIRT(type, name, target) (const type* name = dynamic_cast<const type*>(target.get()))

namespace sirf {

class AsmGenerator {
public:
  explicit AsmGenerator(const IrHolder& holder)
    : holder(holder) {}

  std::string generate();

private:
  int getSizeOf(const IrValue& val);

  std::string getSizePrefix(int size);

  std::string generateRegister_x86_64(const IrValueRegister& reg);
  std::string generateValue_x86_64(const IrValue& val);

  void generateStmt_x86_64(const IrStmt& stat);

private:
  const IrHolder& holder;

  size_t spillOffset = 0;
  const IrStmtFunction* currentFunction = NULL;

  std::ostringstream section_data;
  std::ostringstream section_text;
  std::ostringstream section_bss;
};

} // namespace sirf

#endif
