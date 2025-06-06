// This file is a part of the SIRF (Simple Intermediate Representation Format)
// project Copyright (C) 2025 XnLogical - Licensed under GNU GPL v3.0

#ifndef SIRF_IRSTMTFUNCTION_HPP
#define SIRF_IRSTMTFUNCTION_HPP

#include <Core/Common.hpp>
#include <IR/IrValueSSA.hpp>
#include <IR/IrValueSymbol.hpp>
#include <IR/IrAttribute.hpp>
#include <IR/IrTypeBase.hpp>
#include <IR/IrStmtBase.hpp>
#include <IR/IrValueBase.hpp>

namespace sirf {

struct IrParameter {
  const IrValueSSA id;
  const IrType type;

  explicit IrParameter(IrValueSSA&& id, IrType&& type)
    : id(std::move(id)),
      type(std::move(type)) {}
};

class IrStmtFunction final : public IrStmtBase {
public:
  const IrType retType;
  const IrValueSymbol id;
  const std::vector<IrParameter> params;
  const std::vector<IrAttribute> attribs;
  const IrScope body;

  explicit IrStmtFunction(IrType ret, IrValueSymbol id, std::vector<IrParameter> pars, std::vector<IrAttribute> atts, IrScope body)
    : retType(ret),
      id(id),
      params(pars),
      attribs(atts),
      body(body) {}

  inline static IrStmt newStmt(IrType ret, IrValueSymbol id, std::vector<IrParameter> pars, std::vector<IrAttribute> atts, IrScope body) {
    return std::make_shared<IrStmtFunction>(std::move(ret), std::move(id), std::move(pars), std::move(atts), std::move(body));
  }

  std::string toString() const override;
};

} // namespace sirf

#endif
