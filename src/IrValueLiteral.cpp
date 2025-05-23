// This file is a part of the SIRF (Simple Intermediate Representation Format)
// project Copyright (C) 2025 XnLogical - Licensed under GNU GPL v3.0

#include <SIRF/IR/IrValueLiteral.hpp>

namespace SIRF {

bool IrValueLiteral::isSigned() const {
  using enum IrLiteralKind;

  switch (kind) {
  case i8:
  case i16:
  case i32:
  case i64:
    return true;
  case u8:
  case u16:
  case u32:
  case u64:
    return false;
  default:
    break;
  }

  return false;
}

constexpr bool IrValueLiteral::isLvalue() const {
  return false;
}

std::string IrValueLiteral::toString() const {
  const std::string typeString = SIRF::toString(kind);
  const std::string valueString = SIRF::toString(kind, value);
  return valueString + typeString;
}

std::string toString(IrLiteralKind kind) {
  using enum IrLiteralKind;

  // clang-format off
  switch (kind) {
  case i8:    return "i8";
  case i16:   return "i16";
  case i32:   return "i32";
  case i64:   return "i64";
  case u8:    return "u8";
  case u16:   return "u16";
  case u32:   return "u32";
  case u64:   return "u64";
  case byte:  return "byte";
  case word:  return "word";
  } // clang-format on

  SIRF_UNREACHABLE();
}

std::string toString(IrLiteralKind kind, IrLiteralUn un) {
  using enum IrLiteralKind;

  // clang-format off
  switch (kind) {
  case i8:    return std::to_string(un.i8);
  case i16:   return std::to_string(un.i16);
  case i32:   return std::to_string(un.i32);
  case i64:   return std::to_string(un.i64);
  case u8:    return std::to_string(un.u8);
  case u16:   return std::to_string(un.u16);
  case u32:   return std::to_string(un.u32);
  case u64:   return std::to_string(un.u64);
  case byte:  return std::to_string(un.byte);
  case word:  return std::to_string(un.word);
  } // clang-format on

  SIRF_UNREACHABLE();
}

} // namespace SIRF
