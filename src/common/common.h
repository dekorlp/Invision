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

#ifndef COMMON_H
#define COMMON_H

#include "precompiled.h"


#ifdef INVISION_EXPORTS
#define INVISION_API __declspec(dllexport) 
#else
#define INVISION_API __declspec(dllimport) 
#endif

#include "BaseTypes.h"
#include "WinPlatformInformation.h"
#include "WinLocale.h"
#include "DateTime.h"
#include "TimeSpan.h"
#include "StopWatch.h"
#include "Log.h"

#endif //COMMON_H