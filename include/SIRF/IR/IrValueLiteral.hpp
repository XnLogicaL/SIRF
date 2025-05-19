// This file is a part of the SIRF (Simple Intermediate Representation Format)
// project Copyright (C) 2025 XnLogical - Licensed under GNU GPL v3.0

#ifndef SIRF_IRVALUELITERAL_HPP
#define SIRF_IRVALUELITERAL_HPP

#include <string>
#include <format>
#include <cstdint>

#include <SIRF/Core/APIConfig.hpp>
#include <SIRF/Core/APIMacros.hpp>
#include <SIRF/IR/IrValueBase.hpp>

namespace SIRF {

enum class IrLiteralKind {
  i8,    // Signed 8-bit integer
  i16,   // Signed 16-bit integer
  i32,   // Signed 32-bit integer
  i64,   // Signed 64-bit integer
  u8,    // Unsigned 8-bit integer
  u16,   // Unsigned 16-bit integer
  u32,   // Unsigned 32-bit integer
  u64,   // Unsigned 64-bit integer
  byte,  // One byte
  word,  // Machine word
  dword, // Double machine word
  qword, // Quad machine word
};

union IrLiteralUn {
  int8_t i8;
  int16_t i16;
  int32_t i32;
  int64_t i64;
  uint8_t u8;
  uint16_t u16;
  uint32_t u32;
  uint64_t u64;
  uint8_t byte;
  uint16_t word;
  uint32_t dword;
  uint64_t qword;
};

class IrValueLiteral : public IrValueBase {
public:
  const IrLiteralKind kind;
  const IrLiteralUn value;

  explicit IrValueLiteral(IrLiteralKind kind, IrLiteralUn un)
    : kind(kind),
      value(un) {}

  bool isSigned() const;

  /// Returns whether if the IR literal value be treated as an lvalue
  constexpr bool isLvalue() const override;

  /// Returns the string representation of the IR literal value
  std::string toString() const override;
};

std::string toString(IrLiteralKind kind);
std::string toString(IrLiteralKind kind, IrLiteralUn un);

} // namespace SIRF

#endif
