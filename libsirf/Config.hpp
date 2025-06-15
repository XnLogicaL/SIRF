// This file is a part of the SIRF (Simple Intermediate Representation Format)
// project Copyright (C) 2025 XnLogical - Licensed under GNU GPL v3.0

#ifndef SIRF_APICONFIG_HPP
#define SIRF_APICONFIG_HPP

#include <cassert>

#define SIRF_UNREACHABLE()                                                                                                  \
  do {                                                                                                                      \
    assert(false && "unreachable point reached");                                                                           \
    std::unreachable();                                                                                                     \
  } while (0)

#define SIRF_TODO() assert(false && "todo");

#endif
