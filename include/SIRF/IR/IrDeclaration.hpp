// This file is a part of the SIRF (Simple Intermediate Representation Format)
// project Copyright (C) 2025 XnLogical - Licensed under GNU GPL v3.0

#ifndef SIRF_IRDECLARATION_HPP
#define SIRF_IRDECLARATION_HPP

#include <SIRF/Core/APIConfig.hpp>
#include <SIRF/Core/APIMacros.hpp>
#include <SIRF/IR/IrValueBase.hpp>

namespace SIRF {

enum class IrDeclKind {
  EXTERN,
  GLOBAL,
};

class IrDeclaration {
public:
  const IrDeclKind kind;
  const IrValue value;

  explicit IrDeclaration(IrDeclKind kind, IrValue&& value)
    : kind(kind),
      value(std::move(value)) {}

  std::string toString() const;
};

std::string toString(IrDeclKind kind);

} // namespace SIRF

#endif
