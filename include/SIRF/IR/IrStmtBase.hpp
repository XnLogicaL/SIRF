// This file is a part of the SIRF (Simple Intermediate Representation Format)
// project Copyright (C) 2025 XnLogical - Licensed under GNU GPL v3.0

#ifndef SIRF_IRSTMTBASE_HPP
#define SIRF_IRSTMTBASE_HPP

#include <string>
#include <memory>

#include <SIRF/Core/APIConfig.hpp>
#include <SIRF/Core/APIMacros.hpp>

namespace SIRF {

class IrStmtBase {
public:
  virtual std::string toString() const;
};

using IrStmt = std::shared_ptr<IrStmtBase>;

} // namespace SIRF

#endif
