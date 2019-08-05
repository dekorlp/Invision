#include "precompiled.h"

#ifdef _WIN32

WinPlatformInformation::WinPlatformInformation()
{
	ZeroMemory(&memStatus, sizeof(MEMORYSTATUSEX));
	memStatus.dwLength = sizeof(MEMORYSTATUSEX);
	GlobalMemoryStatusEx(&memStatus);

	ZeroMemory(&SysInfo, sizeof(SYSTEM_INFO));
	GetSystemInfo(&SysInfo);

	ZeroMemory(&osVerInfo, sizeof(OSVERSIONINFOEX));
	osVerInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	GetVersionEx((OSVERSIONINFO*)&osVerInfo);

	IdentifyWindowsVersion();
}

uint64 WinPlatformInformation::GetVirtualRam()
{

	return memStatus.ullTotalVirtual / 1048576;

}

uint64 WinPlatformInformation::GetTotalPhysicalRam()
{
	return memStatus.ullTotalPhys / 1048576;
}

uint64 WinPlatformInformation::GetFreePhysicalRam()
{
	return memStatus.ullAvailPhys / 1048576;
}

uint32 WinPlatformInformation::GetCountOfProcessorCores()
{
	return SysInfo.dwNumberOfProcessors;
}

ProcessorArchitecture WinPlatformInformation::GetProcessorArchitecture()
{
	WINPLATFORM_PROCESSOR_ARCHITECTURE procArchitecture = WINPLATFORM_PROCESSOR_ARCHITECTURE_UNKNOWN;
	if (SysInfo.wProcessorArchitecture == 9)
	{
		procArchitecture = WINPLATFORM_PROCESSOR_ARCHITECTURE_AMD64;
	}
	else if (SysInfo.wProcessorArchitecture == 5)
	{
		procArchitecture = WINPLATFORM_PROCESSOR_ARCHITECTURE_ARM;
	}
	else if (SysInfo.wProcessorArchitecture == 6)
	{
		procArchitecture = WINPLATFORM_PROCESSOR_ARCHITECTURE_IA64;
	}
	else if (SysInfo.wProcessorArchitecture == 0)
	{
		procArchitecture = WINPLATFORM_PROCESSOR_ARCHITECTURE_INTEL;
	}
	return procArchitecture;
}

const char* WinPlatformInformation::CSDVersion()
{
	return osVerInfo.szCSDVersion;
}

const char* WinPlatformInformation::GetOsVersion()
{
	return  WindowsVersion;
}

uint32 WinPlatformInformation::GetOSBuildNumber()
{
	return osVerInfo.dwBuildNumber;
}

void WinPlatformInformation::IdentifyWindowsVersion()
{ 
	const char *WinVer = "";
	
	switch (osVerInfo.dwMajorVersion)
	{
		case 5: // <= WinXP | Win Server 2003 
		{
					switch (osVerInfo.dwMinorVersion)
					{
						case 0:
						{
									WinVer = "Windows 2000";
									break;
						}
						case 1:
						{
									WinVer = "Windows XP";
									break;
						}
						case 2:
						{
								  if (osVerInfo.wProductType == VER_NT_WORKSTATION && SysInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64)
								  {
									  WinVer = "Windows XP Professional x64 Edition";
								  }
								  else if (GetSystemMetrics(SM_SERVERR2) == 0)
								  {
									  WinVer = "Windows Server 2003";
								  }
								  else if (osVerInfo.wSuiteMask & VER_SUITE_WH_SERVER)
								  {
									  WinVer = "Windows Home Server";
								  }
								  else if (GetSystemMetrics(SM_SERVERR2) != 0)
								  {
									  WinVer = "Windows Server 2003 R2";
								  }
								break;
						}
					}
					break;
		}
		case 6: // >=WinVista | WinServer2008
		{
					switch (osVerInfo.dwMinorVersion)
					{
						case 0:
						{
								if (osVerInfo.wProductType == VER_NT_WORKSTATION)
								{
									WinVer = "Windows Vista";
								}
								else
								{
									WinVer = "Windows Server 2008";
								}
									break;
						}
						case 1:
						{
								  if (osVerInfo.wProductType == VER_NT_WORKSTATION)
									{
										WinVer = "Windows 7";
									}
									else
									{
										WinVer = "Windows Server 2008 R2";
									}
									break;
						}
						case 2:
						{
								  if (osVerInfo.wProductType == VER_NT_WORKSTATION)
									{
										WinVer = "Windows 8";
									}
									else
									{
										WinVer = "Windows Server 2012";
									}
									break;
						}
						case 3:
						{
								  if (osVerInfo.wProductType == VER_NT_WORKSTATION)
									{
										WinVer = "Windows 8.1";
									}
									else
									{
										WinVer = "Windows Server 2012 R2";
									}
									break;
						}
					}
					break;
		}
		case 10:// >=Win10 | Windows Server Technical Preview
		{
			switch (osVerInfo.dwMinorVersion)
			{
				case 0:
				{
						if (osVerInfo.wProductType == VER_NT_WORKSTATION)
						{
							WinVer = "Windows 10 Insider Preview";
						}
						else
						{
							WinVer = "Windows Server Technical Preview";
						}
						break;
				}
			}
			break;
		}
	}

	WindowsVersion = WinVer;
}

#endif