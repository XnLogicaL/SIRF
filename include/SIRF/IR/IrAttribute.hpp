// This file is a part of the SIRF (Simple Intermediate Representation Format)
// project Copyright (C) 2025 XnLogical - Licensed under GNU GPL v3.0

#ifndef SIRF_IRATTRIBUTE_HPP
#define SIRF_IRATTRIBUTE_HPP

#include <Core/Common.hpp>
#include <IR/IrValueBase.hpp>

namespace SIRF {

class IrAttribute {
public:
  const std::string id;

  explicit IrAttribute(std::string id)
    : id(id) {}

  std::string toString() const;
};

} // namespace SIRF

#endif
