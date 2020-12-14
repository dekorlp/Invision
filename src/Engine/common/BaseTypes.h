/////////////////////////////////////////////////////////////////////////////
// Name:	  BaseTypes
// Purpose:	  engine specific basetypes
// Author:	  Dennis Koehler
// Created:	  2015-06-03
// Copyright: Copyright (c) 2015, Nano Game Foundation
//			  This unpublished source code is proprietary to Nano Game Foundation.
//			  The source code herein is considered trade secrets and confidential.
//			  Reproduction or distribution, in part or complete, is prohibited except by permission of Nano Game Foundation. 
//			  All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef BASETYPES_H
#define BASETYPES_H

// USED OS
#ifdef _MSC_VER 
#define WIN_32
#endif 

// REAL Type size
#define USE_REAL64

//typedefinitions
typedef unsigned char uint8;
typedef unsigned short int uint16;
typedef unsigned int uint32;
typedef unsigned long long int uint64;

typedef signed char int8;
typedef signed short int int16;
typedef signed int int32;
typedef signed long long int int64;

#ifdef USE_REAL32
typedef float real; // USE_REAL32
#else
typedef double real; // USE_REAL64
#endif

typedef char aChar; // ANSI Character
typedef wchar_t wChar; // WIDE Character
typedef aChar tChar; // 

#endif // BASETYPES_H