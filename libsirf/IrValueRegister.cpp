// This file is a part of the SIRF (Simple Intermediate Representation Format)
// project Copyright (C) 2025 XnLogical - Licensed under GNU GPL v3.0

#include <IR/IrValueRegister.hpp>

namespace sirf {

constexpr bool IrValueRegister::isLvalue() const {
  return false;
}

std::string IrValueRegister::toString() const {
  using enum IrRegisterKind;

  std::string idString = std::to_string(id);

  // clang-format off
  switch (kind) {
  case BYTE:  return 'b' + idString;
  case WORD:  return 'w' + idString;
  case DWORD: return 'x' + idString;
  case QWORD: return 'r' + idString;
  } // clang-format on

  SIRF_UNREACHABLE();
}

} // namespace sirf
