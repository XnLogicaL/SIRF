// This file is a part of the SIRF (Simple Intermediate Representation Format)
// project Copyright (C) 2025 XnLogical - Licensed under GNU GPL v3.0

#ifndef SIRF_APIMACROS_HPP
#define SIRF_APIMACROS_HPP

#if _MSC_VER
#define SIRF_RESTRICT      __restrict
#define SIRF_NOMANGLE      extern "C"
#define SIRF_NODISCARD     [[nodiscard]]
#define SIRF_NORETURN      __declspec(noreturn)
#define SIRF_NOINLINE      __declspec(noinline)
#define SIRF_FORCEINLINE   __forceinline
#define SIRF_OPTIMIZE      __forceinline // MSVC doesn't have 'hot' attribute
#define SIRF_UNREACHABLE() __assume(0)
#define SIRF_FUNCSIG       __FUNCSIG__
#define SIRF_LIKELY(x)     (x) // No branch prediction hints in MSVC
#define SIRF_UNLIKELY(x)   (x)
#else // GCC / Clang
#define SIRF_RESTRICT      __restrict__
#define SIRF_NOMANGLE      extern "C"
#define SIRF_NODISCARD     [[nodiscard]]
#define SIRF_NORETURN      __attribute__((noreturn))
#define SIRF_NOINLINE      __attribute__((noinline))
#define SIRF_FORCEINLINE   inline __attribute__((always_inline))
#define SIRF_OPTIMIZE      inline __attribute__((always_inline, hot))
#define SIRF_UNREACHABLE() __builtin_unreachable()
#define SIRF_FUNCSIG       __PRETTY_FUNCTION__
#define SIRF_LIKELY(a)     (__builtin_expect(!!(a), 1))
#define SIRF_UNLIKELY(a)   (__builtin_expect(!!(a), 0))
#endif

#endif
