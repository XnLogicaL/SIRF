// This file is a part of the SIRF (Simple Intermediate Representation Format)
// project Copyright (C) 2025 XnLogical - Licensed under GNU GPL v3.0

#ifndef SIRF_IRSTMTDECLARATION_HPP
#define SIRF_IRSTMTDECLARATION_HPP

#include <Core/Common.hpp>
#include <IR/IrStmtBase.hpp>
#include <IR/IrValueBase.hpp>

namespace SIRF {

enum class IrDeclKind {
  EXTERN,
  GLOBAL,
};

class IrStmtDeclaration final : public IrStmtBase {
public:
  const IrDeclKind kind;
  const IrValue value;

  explicit IrStmtDeclaration(IrDeclKind kind, IrValue&& value)
    : kind(kind),
      value(std::move(value)) {}

  std::string toString() const override;
};

std::string toString(IrDeclKind kind);

} // namespace SIRF

#endif
