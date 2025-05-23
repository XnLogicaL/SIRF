// This file is a part of the SIRF (Simple Intermediate Representation Format)
// project Copyright (C) 2025 XnLogical - Licensed under GNU GPL v3.0

#include <IR/IrStmtFunction.hpp>

namespace SIRF {

std::string IrStmtFunction::toString() const {
  std::ostringstream oss;
  oss << "fun " << retType->toString();
  oss << ' ' << id.toString() << '(';

  for (size_t i = 0; const IrParameter& param : params) {
    oss << param.id.toString() << ' ' << param.type->toString();
    if (++i < params.size()) {
      oss << ", ";
    }
  }

  oss << ')';
  for (const IrAttribute& attrib : attribs) {
    oss << attrib.toString();
  }

  oss << " {\n";
  for (const IrStmt& stat : body) {
    oss << stat->toString();
  }

  oss << "}\n";
  return oss.str();
}

} // namespace SIRF
