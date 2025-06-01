// This file is a part of the SIRF (Simple Intermediate Representation Format)
// project Copyright (C) 2025 XnLogical - Licensed under GNU GPL v3.0

#ifndef SIRF_IRSTMTBASE_HPP
#define SIRF_IRSTMTBASE_HPP

#include <Core/Common.hpp>

namespace SIRF {

class IrStmtBase {
public:
  virtual std::string toString() const = 0;
};

using IrStmt = std::shared_ptr<IrStmtBase>;
using IrScope = std::vector<IrStmt>;

} // namespace SIRF

#endif
