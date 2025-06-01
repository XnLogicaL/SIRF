// This file is a part of the SIRF (Simple Intermediate Representation Format)
// project Copyright (C) 2025 XnLogical - Licensed under GNU GPL v3.0

#include <Asm/AsmGen.hpp>

namespace SIRF {

std::string AsmGenerator::generate() {
  for (const IrStmt& stat : holder.data) {
#ifdef __x86_64__
    generateTarget_x86_64(stat);
#endif
  }

  std::ostringstream oss;
  oss << "section .data\n";
  oss << section_data.str();

  oss << "section .text\n";
  oss << section_text.str();

  oss << "section .bss\n";
  oss << section_bss.str();

  return oss.str();
}

} // namespace SIRF
