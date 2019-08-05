/////////////////////////////////////////////////////////////////////////////
// Name:	  WinPlatform
// Purpose:	  
// Author:	  Dennis Koehler
// Created:	  2015-06-05
// Copyright: Copyright (c) 2015, Nano Game Foundation
//			  This unpublished source code is proprietary to Nano Game Foundation.
//			  The source code herein is considered trade secrets and confidential.
//			  Reproduction or distribution, in part or complete, is prohibited except by permission of Nano Game Foundation. 
//			  All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifdef _WIN32

#ifndef WINPLATFORMINFORMATION_H
#define WINPLATFORMINFORMATION_H

#pragma warning(disable: 4996)

typedef enum WINPLATFORM_PROCESSOR_ARCHITECTURE
{
	WINPLATFORM_PROCESSOR_ARCHITECTURE_AMD64 = 9,
	WINPLATFORM_PROCESSOR_ARCHITECTURE_ARM = 8,
	WINPLATFORM_PROCESSOR_ARCHITECTURE_IA64 = 6,
	WINPLATFORM_PROCESSOR_ARCHITECTURE_INTEL = 0,
	WINPLATFORM_PROCESSOR_ARCHITECTURE_UNKNOWN = 0xffff
} ProcessorArchitecture;

class HAL_API WinPlatformInformation
{
public:
	WinPlatformInformation();
	uint64 GetVirtualRam();
	uint64 GetTotalPhysicalRam();
	uint64 GetFreePhysicalRam();
	uint32 GetOSBuildNumber();
	const char* GetOsVersion();
	const char* GetLocaleLanguage();
	const char* CSDVersion();
	const char* GetLocaleCountry();
	const char* GetLocaleGeoCountry();
	uint32 GetCountOfProcessorCores();
	WINPLATFORM_PROCESSOR_ARCHITECTURE GetProcessorArchitecture();
	
	uint32 IdentifyCID(WCHAR * langISO3166);

private:
	MEMORYSTATUSEX memStatus;
	OSVERSIONINFOEX osVerInfo;
	SYSTEM_INFO SysInfo;
	const char* WindowsVersion;
	void IdentifyWindowsVersion();
};

#endif //WINPLATFORMINFORMATION_H

#endif