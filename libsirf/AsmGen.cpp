// This file is a part of the SIRF (Simple Intermediate Representation Format)
// project Copyright (C) 2025 XnLogical - Licensed under GNU GPL v3.0

#include <Asm/AsmGen.hpp>
#include <iostream>

namespace sirf {

enum LinkerId {
  LNONE = 0,
  LGNU, // mainly for Linux
  LCL,  // for Windows
};

static constexpr LinkerId getLinkerId() {
#ifdef __linux__
  return LGNU;
#elifdef _WIN32
  return LCL;
#endif

  return LNONE;
}

int AsmGenerator::getSizeOf(const IrValue& val) {
  if SIRF_CHECKVIRT (IrValueLiteral, lit, val) {
    if SIRF_CHECKVIRT (IrTypeSized, size, lit->type) {
      return size->size;
    }
  }

  return -1;
}

std::string AsmGenerator::getSizePrefix(int size) {
  switch (size) {
  case 8:
    return "byte";
  case 16:
    return "word";
  case 32:
    return "dword";
  case 64:
    return "qword";
  default:
    break;
  }

  return "";
}

std::string AsmGenerator::generate() {
  for (const IrStmt& stat : holder.data) {
#ifdef __x86_64__
    generateStmt_x86_64(stat);
#else
#error "Unsupported target"
#endif
  }

  std::ostringstream oss;
  oss << "section .data\n";
  oss << section_data.str();

  oss << "section .text\n";
  oss << section_text.str();

  oss << "section .bss\n";
  oss << section_bss.str();

  // GNU executable stack warning workaround
  if (getLinkerId() == LGNU) {
    oss << "section .note.GNU-stack,\"\",@progbits\n";
  }

  return oss.str();
}

} // namespace sirf
