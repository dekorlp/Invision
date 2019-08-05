/////////////////////////////////////////////////////////////////////////////
// Name:	  PlatformCPUFrequency
// Purpose:	  estimates the CPUs frequency
// Author:	  Dennis Koehler
// Created:	  2014-06-16
// Copyright: Copyright (c) 2015, Nano Game Foundation
//			  This unpublished source code is proprietary to Nano Game Foundation.
//			  The source code herein is considered trade secrets and confidential.
//			  Reproduction or distribution, in part or complete, is prohibited except by permission of Nano Game Foundation. 
//			  All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef PLATFORMCPUFREQUENCY_H
#define PLATFORMCPUFREQUENCY_H

#include "PlatformCPUID.h"

class HAL_API PlatformCPUFrequency
{
private:
#ifdef _WIN32
	DWORD ReadCPUSpeedFromRegistry();
#elif

	PlatformCPU CpuPlatform;
	int32 Milliseconds;
	int32 Milliseconds0;
	int64 Ticks;
	int64 Ticks0;

	void StartTimingCPU();
	void UpdateCPUTime();
	int32 GetMilliseconds();
	int64 GetTicks();
	void Sleep(unsigned int mseconds);
#endif

public:
	real EstimateCpuSpeed();
};

#endif // PLATFORMCPUFREQUENCY_H