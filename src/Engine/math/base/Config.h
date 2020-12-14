#ifndef CONFIG_H
#define CONFIG_H



#if _MSC_VER >= 1910 && _MSC_VER <= 1916
#define SSE_INSTR_SET 7
#else
#define SSE_INSTR_SET 5
#endif

#ifdef _WIN64
#define USE_SSE2
#endif //_WIN64

#define PI 3.14159265358979323846264
#define PI_F 3.14159265358979f

#// Include the appropriate header file for intrinsic functions
#if SSE_INSTR_SET > 7                  // AVX2 and later
#ifdef __GNUC__
#include <x86intrin.h>         // x86intrin.h includes header files for whatever instruction
// sets are specified on the compiler command line, such as:
// xopintrin.h, fma4intrin.h
#else
#include <immintrin.h>         // MS version of immintrin.h covers AVX, AVX2 and FMA3
#endif // __GNUC__
#elif SSE_INSTR_SET == 7
#include <immintrin.h>             // AVX
#elif SSE_INSTR_SET == 6
#include <nmmintrin.h>             // SSE4.2
#elif SSE_INSTR_SET == 5
#include <smmintrin.h>             // SSE4.1
#elif SSE_INSTR_SET == 4
#include <tmmintrin.h>             // SSSE3
#elif SSE_INSTR_SET == 3
#include <pmmintrin.h>             // SSE3
#elif SSE_INSTR_SET == 2
#define USE_SSE2
#include <emmintrin.h>             // SSE2
#elif SSE_INSTR_SET == 1
#include <xmmintrin.h>             // SSE

#endif // SIMD_CONFIG_H

#endif // CONFIG_H

