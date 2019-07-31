/////////////////////////////////////////////////////////////////////////////
// Name:	  Hal
// Purpose:	  includes addition headers, dll definition
// Author:	  Dennis Koehler
// Created:	  2015-06-03
// Copyright: Copyright (c) 2015, Nano Game Foundation
//			  This unpublished source code is proprietary to Nano Game Foundation.
//			  The source code herein is considered trade secrets and confidential.
//			  Reproduction or distribution, in part or complete, is prohibited except by permission of Nano Game Foundation. 
//			  All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef LOWLEVEL_H
#define LOWLEVEL_H

#include "precompiled.h"

#ifdef HAL_EXPORTS
#define HAL_API __declspec(dllexport) 
#else
#define HAL_API __declspec(dllimport) 
#endif
#include "PlatformCPUID.h"
#include "PlatformCPUFrequency.h"
#include "PlatformHardDisk.h"
#include "InStringA.h"
#include "InStringW.h"
//#include "Allocator.h"
//#include "LinearAllocator.h"
//#include "StackAllocator.h"
//#include "FreeListAllocator.h"
//#include "StandardBoundsChecking.h"
//#include "NoBoundsChecking.h"
//#include "DefaultMemoryTracking.h"
//#include "AdvancedMemoryTracking.h"



#endif //LOWLEVEL_H