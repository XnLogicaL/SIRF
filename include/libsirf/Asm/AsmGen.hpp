// This file is a part of the SIRF (Simple Intermediate Representation Format)
// project Copyright (C) 2025 XnLogical - Licensed under GNU GPL v3.0

#ifndef SIRF_ASMGENBASE_HPP
#define SIRF_ASMGENBASE_HPP

#include <Core/Common.hpp>
#include <IR/IrHolder.hpp>
#include <IR/IrValues.hpp>
#include <IR/IrTypes.hpp>
#include <IR/IrStmts.hpp>
#include <magic_enum/magic_enum.hpp>
#include <arena.h>

#define SIRF_CHECKVIRT(type, name, target) (const type* name = dynamic_cast<const type*>(target))

namespace sirf {

enum class StkIdKind {
  spill,
  variable,
};

struct StkReg {
  size_t offset;
};

struct StkVar {
  size_t offset;
  size_t size;
};

struct StkId {
  StkIdKind kind;
  union {
    StkReg reg;
    StkVar var;
  } u;
};

struct ValueRepr {
  const bool isPointer;
  const std::string val;
  const IrValueBase* og_val;
};

class AsmGenerator {
public:
  explicit AsmGenerator(const IrHolder& holder)
    : holder(holder) {}

  std::string generate();

private:
  int getSizeOf(const IrValueBase* val);

  const char* getSizePrefix(int size);

  // x86-64 stuff
  size_t alloca_x86_64(size_t size);

  void generateBinaryInstruction_x86_64(const std::string& opName, const ValueRepr& dst, const ValueRepr& src);

  ValueRepr generateRegister_x86_64(const IrValueRegister& reg);
  ValueRepr generateVariable_x86_64(const IrValueSSA& var);
  ValueRepr generateValue_x86_64(const IrValue& val);

  void generateStmt_x86_64(const IrStmt& stat);

private:
  const IrHolder& holder;

  size_t stackOffset = 0;
  size_t allocaRemBytes = 0;
  const IrStmtFunction* currentFunction = NULL;

  std::vector<std::unordered_map<uint32_t, StkId>> stackMap;

  std::ostringstream section_data;
  std::ostringstream section_text;
  std::ostringstream section_bss;
};

} // namespace sirf

#endif
