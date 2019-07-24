#include "precompiled.h"
#include "PlatformCPUID.h"


	PlatformCPU::PlatformCPU()
	{
		SetCpuSupport();
		SetMaxFunctions();
		SetCPUVendor();
		SetCPUName();

		SetCPUInformation();
		
	}

	void PlatformCPU::SetCpuSupport()
	{
		uint32 CPUIsSupported = false;

#ifndef _WIN64
		__asm
		{
			pushfd
				pop eax
				mov ebx, eax
				xor eax, (1 << 21)
				push eax
				popfd
				pushfd
				pop eax
				xor eax, ebx
				mov CPUIsSupported, eax
		}
#endif //_WIN64
		if (CPUIsSupported != 0)
		{
			CpuIdSupport = CPUID_IS_AVAILABLE;
		}
		else
		{
			CpuIdSupport = CPUID_IS_NOT_AVAILABLE;
		}
	}

	CPUIDSUPPORT PlatformCPU::IsCPUIDSupported()
	{
		return CpuIdSupport;
	}

	void PlatformCPU::SetMaxFunctions()
	{
		if (IsCPUIDSupported() == CPUID_IS_NOT_AVAILABLE)
		{
			return;
		}

		uint32 CPUInfo[4] = { 0, 0, 0, 0 };
		uint32 _highestFunction = 0;
		uint32 _highestAdvancedFunction = 0;
#ifndef _WIN64
		__asm
		{
			mov eax, 0x0000000
				cpuid
				mov CPUInfo[0], eax
				mov CPUInfo[4], ebx
				mov CPUInfo[8], edx
				mov CPUInfo[12], ecx
				mov _highestFunction, eax
		};
#endif //_WIN64
		HighestFunction = _highestFunction;
#ifndef _WIN64
		__asm
		{
			mov eax, 0x80000000
				cpuid
				mov _highestAdvancedFunction, eax
		};
#endif //_WIN64
		HighestAdvancedFunction = _highestAdvancedFunction;
	}

	void PlatformCPU::SetCPUVendor()
	{
		if (IsCPUIDSupported() == CPUID_IS_NOT_AVAILABLE)
		{
			return;
		}

		uint32 CPUInfo[4];
#ifndef _WIN64
		__asm
		{
			mov eax, 0x0000000
				cpuid
				mov CPUInfo[0], ebx
				mov CPUInfo[4], edx
				mov CPUInfo[8], ecx
		}
#endif //_WIN64

		uint32 charpos = 0;
		VendorAlias = (char*)malloc(sizeof(CPUInfo));

		for (uint32 i = 0; i < (sizeof(CPUInfo)-sizeof(uint32)) / sizeof(uint32); i++)
		{
			for (uint32 j = 0; j < sizeof(uint32); j++)
			{
				VendorAlias[charpos] = char((CPUInfo[i] >> j * 8) & 0xFF);
				charpos++;
			}
		}
		VendorAlias[charpos] = char('\0');
	}

	void PlatformCPU::SetCPUName()
	{
		if (IsCPUIDSupported() == CPUID_IS_NOT_AVAILABLE)
		{
			return;
		}

		uint32 cpuName[12];
#ifndef _WIN64
		__asm
		{
			mov eax, 0x80000002
				cpuid
				mov cpuName[0], eax
				mov cpuName[4], ebx
				mov cpuName[8], ecx
				mov cpuName[12], edx

				mov eax, 0x80000003
				cpuid
				mov cpuName[16], eax
				mov cpuName[20], ebx
				mov cpuName[24], ecx
				mov cpuName[28], edx

				mov eax, 0x80000004
				cpuid
				mov cpuName[32], eax
				mov cpuName[36], ebx
				mov cpuName[40], ecx
				mov cpuName[44], edx
		};
#endif //_WIN64

		uint32 charpos = 0;
		CpuName = (char*)malloc(sizeof(cpuName));
		for (uint32 i = 0; i < sizeof(cpuName) / sizeof(uint32); i++)
		{
			for (uint32 j = 0; j < sizeof(uint32); j++)
			{
				CpuName[charpos] = char((cpuName[i] >> j * 8) & 0xFF);
				charpos++;
			}
		}
		CpuName[charpos] = char('\0');

	}


	bool PlatformCPU::SetCPUInformation()
	{
		uint32 CPUEAX = 0;
		uint32 CPUEBX = 0;
		uint32 CPUECX = 0;
		uint32 CPUEDX = 0;

		if (IsCPUIDSupported() == CPUID_IS_NOT_AVAILABLE)
		{
			return false;
		}

#ifndef _WIN64
		__asm
		{
			mov eax, 0x00000001
				cpuid
				mov CPUEAX, eax
				mov CPUEBX, ebx
				mov CPUECX, ecx
				mov CPUEDX, edx
		};
#endif //_WIN64

		CpuInformation.CpuName = CpuName;
		
		if (strcmp(VendorAlias, "GenuineIntel") == 0)
		{
			CpuInformation.CpuVendor = INTEL;
			CpuInformation.Vendor = "Intel";
		}
		else if (strcmp(VendorAlias, "AuthenticAMD") == 0)
		{
			CpuInformation.CpuVendor = AMD;
			CpuInformation.Vendor = "Amd";
		}
		else if (strcmp(VendorAlias, "CyrixInstead") == 0)
		{
			CpuInformation.CpuVendor = CYRIX;
			CpuInformation.Vendor = "Cyrix";
		}
		else if (strcmp(VendorAlias, "CentaurHauls") == 0)
		{
			CpuInformation.CpuVendor = CENTAUR;
			CpuInformation.Vendor = "Centaur";
		}
		else if (strcmp(VendorAlias, "GenuineTMx86") == 0)
		{
			CpuInformation.CpuVendor = TRANSMETA;
			CpuInformation.Vendor = "Transmeta";
		}
		else if (strcmp(VendorAlias, "SiS SiS SiS ") == 0)
		{
			CpuInformation.CpuVendor = SIS;
			CpuInformation.Vendor = "Sis";
		}
		else if (strcmp(VendorAlias, "UMC UMC UMC ") == 0)
		{
			CpuInformation.CpuVendor = UMC;
			CpuInformation.Vendor = "Umc";
		}
		else if (strcmp(VendorAlias, "NexGenDriven") == 0)
		{
			CpuInformation.CpuVendor = NEXGEN;
			CpuInformation.Vendor = "Nexgen";
		}
		else if (strcmp(VendorAlias, "RiseRiseRise") == 0)
		{
			CpuInformation.CpuVendor = RISE;
			CpuInformation.Vendor = "Rise";
		}
		else if (strcmp(VendorAlias, "Geode by NSC") == 0)
		{
			CpuInformation.CpuVendor = NATIONAL_SEMICONDUCTOR;
			CpuInformation.Vendor = "National Semiconductor";
		}
		else
		{
			CpuInformation.CpuVendor = UNKNOWNCPUVENDOR;
			CpuInformation.Vendor = "";
		}

		CpuInformation.Stepping = (CPUEAX >> 0) & 0xF;
		CpuInformation.Model = (CPUEAX >> 4) & 0xF;
		CpuInformation.Family = (CPUEAX >> 8) & 0xF;
		CpuInformation.Intel.Type = (CPUEAX >> 12) & 0x3;
		CpuInformation.AdvancedModel = (CPUEAX >> 16) & 0xF;
		CpuInformation.AdvancedFamily = (CPUEAX >> 20) & 0xFF;

		CpuInformation.BrandID = CPUEBX & 0xFF;
		CpuInformation.CFlushSize = (CPUEBX >> 8) & 0xFF;
		CpuInformation.CountLogicalProcessors = (CPUEBX >> 16) & 0xFF;
		CpuInformation.APICID = (CPUEBX >> 24) & 0xFF;

		CpuInformation.FPU = CPUEDX & 0x1;
		CpuInformation.VME = (CPUEDX >> 1) & 0x1;
		CpuInformation.DE = (CPUEDX >> 2) & 0x1;
		CpuInformation.PSE = (CPUEDX >> 3) & 0x1;
		CpuInformation.TSC = (CPUEDX >> 4) & 0x1;
		CpuInformation.MSR = (CPUEDX >> 5) & 0x1;
		CpuInformation.PAE = (CPUEDX >> 6) & 0x1;
		CpuInformation.MCE = (CPUEDX >> 7) & 0x1;
		CpuInformation.CX8 = (CPUEDX >> 8) & 0x1;
		CpuInformation.APIC = (CPUEDX >> 9) & 0x1;
		CpuInformation.SEP = (CPUEDX >> 11) & 0x1;
		CpuInformation.MTRR = (CPUEDX >> 12) & 0x1;
		CpuInformation.PGE = (CPUEDX >> 13) & 0x1;
		CpuInformation.MCA = (CPUEDX >> 14) & 0x1;
		CpuInformation.CMOV = (CPUEDX >> 15) & 0x1;
		CpuInformation.PAT = (CPUEDX >> 16) & 0x1;
		CpuInformation.PSE36 = (CPUEDX >> 17) & 0x1;
		CpuInformation.Intel.PSN = (CPUEDX >> 18) & 0x1;
		CpuInformation.CLFL = (CPUEDX >> 19) & 0x1;
		CpuInformation.Intel.DTES = (CPUEDX >> 21) & 0x1;
		CpuInformation.Intel.ACPI = (CPUEDX >> 22) & 0x1;
		CpuInformation.MMX = (CPUEDX >> 23) & 0x1;
		CpuInformation.FXSR = (CPUEDX >> 24) & 0x1;
		CpuInformation.SSE = (CPUEDX >> 25) & 0x1;
		CpuInformation.SSE2 = (CPUEDX >> 26) & 0x1;
		CpuInformation.Intel.SS = (CPUEDX >> 27) & 0x1;
		CpuInformation.HTT = (CPUEDX >> 28) & 0x1;
		CpuInformation.Intel.TM1 = (CPUEDX >> 29) & 0x1;
		CpuInformation.Intel.IA64 = (CPUEDX >> 30) & 0x1;
		CpuInformation.Intel.PBE = (CPUEDX >> 31) & 0x1;

		if (CpuInformation.CpuVendor == AMD &&GetHighestAdvancedFunction() >= 0x80000001)
		{
#ifndef _WIN64
			__asm
			{
				mov eax, 0x80000001
					cpuid
					mov CPUEAX, eax
					mov CPUEBX, ebx
					mov CPUECX, ecx
					mov CPUEDX, edx
			};
#endif //_WIN64

			CpuInformation.Amd.SYSCALL = (CPUEDX >> 11) & 0x1;
			CpuInformation.Amd.XD = (CPUEDX >> 20) & 0x1;
			CpuInformation.Amd.MMXEXT = (CPUEDX >> 22) & 0x1;
			CpuInformation.Amd.FFXSR = (CPUEDX >> 25) & 0x1;
			CpuInformation.Amd.LM = (CPUEDX >> 29) & 0x1;
			CpuInformation.Amd.DDNOWEXT = (CPUEDX >> 30) & 0x1;
			CpuInformation.Amd.DDNOW = (CPUEDX >> 31) & 0x1;
		}
		else
		{
			CpuInformation.Amd.SYSCALL = false;
			CpuInformation.Amd.XD = false;
			CpuInformation.Amd.MMXEXT = false;
			CpuInformation.Amd.FFXSR = false;
			CpuInformation.Amd.LM = false;
			CpuInformation.Amd.DDNOWEXT = false;
			CpuInformation.Amd.DDNOW = false;
		}
		return true;
	}


	const char* PlatformCPU::GetCPUVendor()
	{
		return VendorAlias;
	}

	const char* PlatformCPU::GetCpuName()
	{
		return CpuName;
	}

	uint32 PlatformCPU::GetHighestFunction()
	{
		return HighestFunction;
	}
	uint32 PlatformCPU::GetHighestAdvancedFunction()
	{
		return HighestAdvancedFunction;
	}

	CPUINFORMATION PlatformCPU::GetCPUInformation()
	{
		return CpuInformation;
	}