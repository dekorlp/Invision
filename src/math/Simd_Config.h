/////////////////////////////////////////////////////////////////////////////
// Name:	  Simd Konfiguration
// Purpose:	  A Wrapper for SSE
// Author:	  Dennis Koehler
// Created:	  2018-03-09
// Copyright: Copyright (c) 2015, Nano Game Foundation
//			  This unpublished source code is proprietary to Nano Game Foundation.
//			  The source code herein is considered trade secrets and confidential.
//			  Reproduction or distribution, in part or complete, is prohibited except by permission of Nano Game Foundation. 
//			  All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef SIMD_CONFIG_H
#define SIMD_CONFIG_H


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



#endif