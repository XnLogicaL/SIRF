// This file is a part of the SIRF (Simple Intermediate Representation Format)
// project Copyright (C) 2025 XnLogical - Licensed under GNU GPL v3.0

#ifndef SIRF_APIMACROS_HPP
#define SIRF_APIMACROS_HPP

#define SIRF_NOMANGLE extern "C"

#if _MSC_VER
#define SIRF_RESTRICT      __restrict
#define SIRF_NORETURN      __declspec(noreturn)
#define SIRF_NOINLINE      __declspec(noinline)
#define SIRF_FORCEINLINE   inline __forceinline
#define SIRF_UNREACHABLE() __assume(0)
#define SIRF_FUNCSIG       __FUNCSIG__
#define SIRF_LIKELY(x)     (x) // No branch prediction hints in MSVC
#define SIRF_UNLIKELY(x)   (x)
#else // GCC / Clang
#define SIRF_RESTRICT      __restrict__
#define SIRF_NORETURN      __attribute__((noreturn))
#define SIRF_NOINLINE      __attribute__((noinline))
#define SIRF_FORCEINLINE   inline __attribute__((always_inline))
#define SIRF_UNREACHABLE() __builtin_unreachable()
#define SIRF_FUNCSIG       __PRETTY_FUNCTION__
#define SIRF_LIKELY(a)     (__builtin_expect(!!(a), 1))
#define SIRF_UNLIKELY(a)   (__builtin_expect(!!(a), 0))
#endif

// Check if libstacktrace is available.
#if __has_include(<stacktrace>) && __cplusplus >= 202302L
#include <stacktrace>
#define SIRF_HASSTACKTRACE 1
#else
#define SIRF_HASSTACKTRACE 0
#endif

#if SIRF_HASSTACKTRACE == 1
#define SIRF_STACKTRACE std::stacktrace::current()
#else
#define SIRF_STACKTRACE ""
#endif

#define SIRF_ASSERT(condition, message)                                                                                                              \
  if (!(condition)) {                                                                                                                                \
    std::cerr << "XVM_ASSERT(): assertion '" << #condition << "' failed.\n"                                                                          \
              << "location: " << __FILE__ << ":" << __LINE__ << "\nmessage: " << message << "\n";                                                    \
    if (SIRF_HASSTACKTRACE) {                                                                                                                        \
      std::cerr << "cstk:\n" << SIRF_STACKTRACE << '\n';                                                                                             \
    }                                                                                                                                                \
    std::abort();                                                                                                                                    \
  }

#define SIRF_TODO() SIRF_ASSERT(false, "todo");

#endif
