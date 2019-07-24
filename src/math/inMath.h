/////////////////////////////////////////////////////////////////////////////
// Name:	  QuaSystemCore
// Purpose:	  includes header files, dll definition
// Author:	  Dennis Koehler
// Created:	  2014-05-03
// Copyright: Copyright (c) 2015, Nano Game Foundation
//			  This unpublished source code is proprietary to Nano Game Foundation.
//			  The source code herein is considered trade secrets and confidential.
//			  Reproduction or distribution, in part or complete, is prohibited except by permission of Nano Game Foundation. 
//			  All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef INMATH_H
#define INMATH_H

#include "precompiled.h"

#ifdef CORE_EXPORTS
#define CORE_API __declspec(dllexport) 
#else
#define CORE_API __declspec(dllimport) 
#endif

#define SSE_INSTR_SET 5

#ifdef _WIN64
#define USE_SSE2
#endif //_WIN64

#define PI 3.14159265358979323846264



#pragma warning(push)
#pragma warning(disable:4799)
#include "Simd_Config.h"
#pragma warning(pop)


#include "sse_mathfun.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Matrix.h"
#include "Quaternion.h"


#endif // INMATH_H