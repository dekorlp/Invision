#include "precompiled.h"
#include "PlatformCPUIDInformation.h"
/*
void PlatformCPUInformation::CountCPUCores(uint16 *physicalCores, uint16 *virtualCores)
{
	// TODO Count of Cores eventually faulty
	if (cpuInformation.cpuVendor == INTEL && cpuInformation.HTT == true && cpuPlatform.GetHighestFunction() >= 0x0000000B)
	{
		uint8 countVirtCpuPerPhysCore = 0;
		uint16 virtualCpu = 0;
		uint16 physicalCpu = 0;
		__asm
		{
			mov eax, 0x0000000B
				mov ecx, 0x00000000
				cpuid
				shr ebx, 0
				and ebx, 0xFF
				mov countVirtCpuPerPhysCore, bl

				mov eax, 0x0000000B
				mov ecx, 0x00000001
				cpuid
				shr ebx, 0
				and ebx, 0xFF
				mov virtualCpu, bx
				mov al, bl // make ready for division!
				div countVirtCpuPerPhysCore
				mov physicalCpu, ax
		}
		*physicalCores = physicalCpu;
		*virtualCores = virtualCpu;
	}
	else if (cpuInformation.cpuVendor == INTEL && cpuInformation.HTT == true && cpuPlatform.GetHighestFunction() >= 0x00000004)
	{
		int32 numApicIds = 0;
		int32 count = cpuInformation.CountLogicalProcessors;
		int32 temp = 0;

		if (count % 2 != 0)
		{
			count = count + 1;
		}

		__asm
		{
			mov eax, 0x00000004
				mov ecx, 0x00000000
				cpuid
				shr eax, 26
				and eax, 0x3F
				mov numApicIds, eax
		}
		temp = numApicIds;
		temp = temp >> count;
		if (temp % 2 != 0)
		{
			temp = temp + 1;
		}


		*physicalCores = temp;
		*virtualCores = count;
	}
	else if (cpuInformation.cpuVendor == INTEL && cpuInformation.HTT == true && cpuPlatform.GetHighestFunction() < 0x00000004)
	{
		int32 count = cpuInformation.CountLogicalProcessors;
		if (count % 2 != 0)
		{
			count = count + 1;
		}

		*physicalCores = 1;
		*virtualCores = count;
	}
	else if (cpuInformation.cpuVendor == INTEL && cpuInformation.HTT == false)
	{
		*physicalCores = 0;
		*virtualCores = 0;
	}

	if (cpuInformation.cpuVendor == AMD && cpuInformation.HTT == true && cpuPlatform.GetHighestAdvancedFunction() >= 0x80000008)
	{
		uint32 coreBits = 0;
		uint32 logicalBits = 0;
		uint32 count = cpuInformation.CountLogicalProcessors;

		if (count % 2 != 0)
		{
			count = count + 1;
		}

		__asm
		{
			mov eax, 0x80000008
				cpuid
				shr ecx, 12
				and ecx, 0xF
				mov coreBits, ecx
		}

		if (coreBits == 0)
		{
			__asm
			{
				mov eax, 0x80000008
					cpuid
					shr ecx, 0
					and ecx, 0xFF
					mov coreBits, ecx
			}
		}
		logicalBits = coreBits << count;

		*physicalCores = coreBits;
		*virtualCores = logicalBits;
	}
	else if (cpuInformation.cpuVendor == AMD && cpuInformation.HTT == true && cpuPlatform.GetHighestAdvancedFunction() < 0x80000008)
	{
		uint32 count = cpuInformation.CountLogicalProcessors;

		if (count % 2 != 0)
		{
			count = count + 1;
		}

		*physicalCores = count;
		*virtualCores = 0;
	}
}

bool PlatformCPUInformation::SetCPUInformation()
{
	uint32 CPUEAX = 0;
	uint32 CPUEBX = 0;
	uint32 CPUECX = 0;
	uint32 CPUEDX = 0;

	if (cpuPlatform.IsCPUIDSupported() == CPUID_IS_NOT_AVAILABLE)
	{
		return false;
	}

	SetVendor();

	__asm
	{
		mov eax, 0x00000001
			cpuid
			mov CPUEAX, eax
			mov CPUEBX, ebx
			mov CPUECX, ecx
			mov CPUEDX, edx
	};

	cpuInformation.Stepping = (CPUEAX >> 0) & 0xF;
	cpuInformation.Model = (CPUEAX >> 4) & 0xF;
	cpuInformation.Family = (CPUEAX >> 8) & 0xF;
	cpuInformation.Intel.Type = (CPUEAX >> 12) & 0x3;
	cpuInformation.AdvancedModel = (CPUEAX >> 16) & 0xF;
	cpuInformation.AdvancedFamily = (CPUEAX >> 20) & 0xFF;

	cpuInformation.BrandID = CPUEBX & 0xFF;
	cpuInformation.CFlushSize = (CPUEBX >> 8) & 0xFF;
	cpuInformation.CountLogicalProcessors = (CPUEBX >> 16) & 0xFF;
	cpuInformation.APICID = (CPUEBX >> 24) & 0xFF;

	cpuInformation.FPU = CPUEDX & 0x1;
	cpuInformation.VME = (CPUEDX >> 1) & 0x1;
	cpuInformation.DE = (CPUEDX >> 2) & 0x1;
	cpuInformation.PSE = (CPUEDX >> 3) & 0x1;
	cpuInformation.TSC = (CPUEDX >> 4) & 0x1;
	cpuInformation.MSR = (CPUEDX >> 5) & 0x1;
	cpuInformation.PAE = (CPUEDX >> 6) & 0x1;
	cpuInformation.MCE = (CPUEDX >> 7) & 0x1;
	cpuInformation.CX8 = (CPUEDX >> 8) & 0x1;
	cpuInformation.APIC = (CPUEDX >> 9) & 0x1;
	cpuInformation.SEP = (CPUEDX >> 11) & 0x1;
	cpuInformation.MTRR = (CPUEDX >> 12) & 0x1;
	cpuInformation.PGE = (CPUEDX >> 13) & 0x1;
	cpuInformation.MCA = (CPUEDX >> 14) & 0x1;
	cpuInformation.CMOV = (CPUEDX >> 15) & 0x1;
	cpuInformation.PAT = (CPUEDX >> 16) & 0x1;
	cpuInformation.PSE36 = (CPUEDX >> 17) & 0x1;
	cpuInformation.Intel.PSN = (CPUEDX >> 18) & 0x1;
	cpuInformation.CLFL = (CPUEDX >> 19) & 0x1;
	cpuInformation.Intel.DTES = (CPUEDX >> 21) & 0x1;
	cpuInformation.Intel.ACPI = (CPUEDX >> 22) & 0x1;
	cpuInformation.MMX = (CPUEDX >> 23) & 0x1;
	cpuInformation.FXSR = (CPUEDX >> 24) & 0x1;
	cpuInformation.SSE = (CPUEDX >> 25) & 0x1;
	cpuInformation.SSE2 = (CPUEDX >> 26) & 0x1;
	cpuInformation.Intel.SS = (CPUEDX >> 27) & 0x1;
	cpuInformation.HTT = (CPUEDX >> 28) & 0x1;
	cpuInformation.Intel.TM1 = (CPUEDX >> 29) & 0x1;
	cpuInformation.Intel.IA64 = (CPUEDX >> 30) & 0x1;
	cpuInformation.Intel.PBE = (CPUEDX >> 31) & 0x1;

	if (cpuInformation.cpuVendor == AMD &&cpuPlatform.GetHighestAdvancedFunction() >= 0x80000001)
	{
		__asm
		{
			mov eax, 0x80000001
				cpuid
				mov CPUEAX, eax
				mov CPUEBX, ebx
				mov CPUECX, ecx
				mov CPUEDX, edx
		};
		cpuInformation.Amd.SYSCALL = (CPUEDX >> 11) & 0x1;
		cpuInformation.Amd.XD = (CPUEDX >> 20) & 0x1;
		cpuInformation.Amd.MMXEXT = (CPUEDX >> 22) & 0x1;
		cpuInformation.Amd.FFXSR = (CPUEDX >> 25) & 0x1;
		cpuInformation.Amd.LM = (CPUEDX >> 29) & 0x1;
		cpuInformation.Amd.DDNOWEXT = (CPUEDX >> 30) & 0x1;
		cpuInformation.Amd.DDNOW = (CPUEDX >> 31) & 0x1;
	}
	else
	{
		cpuInformation.Amd.SYSCALL = false;
		cpuInformation.Amd.XD = false;
		cpuInformation.Amd.MMXEXT = false;
		cpuInformation.Amd.FFXSR = false;
		cpuInformation.Amd.LM = false;
		cpuInformation.Amd.DDNOWEXT = false;
		cpuInformation.Amd.DDNOW = false;
	}


	uint16 physicalCores = 0;
	uint16 virtualCores = 0;

	CountCPUCores(&physicalCores, &virtualCores);

	cpuInformation.PhysicalCoresCount = physicalCores;
	cpuInformation.VirtualCoresCount = virtualCores;
	cpuInformation.CpuName = cpuPlatform.GetCpuName();

	return true;
}

void PlatformCPUInformation::SetVendor()
{
	cpuPlatform.SetCPUVendor();
	cpuInformation.VendorAlias = cpuPlatform.GetCPUVendor();

	if (cpuPlatform.GetCPUVendor() == "GenuineIntel")
	{
		cpuInformation.cpuVendor = INTEL;
		cpuInformation.Vendor = "Intel";
	}
	else if (cpuPlatform.GetCPUVendor() == "AuthenticAMD")
	{
		cpuInformation.cpuVendor = AMD;
		cpuInformation.Vendor = "Amd";
	}
	else if (cpuPlatform.GetCPUVendor() == "CyrixInstead")
	{
		cpuInformation.cpuVendor = CYRIX;
		cpuInformation.Vendor = "Cyrix";
	}
	else if (cpuPlatform.GetCPUVendor() == "CentaurHauls")
	{
		cpuInformation.cpuVendor = CENTAUR;
		cpuInformation.Vendor = "Centaur";
	}
	else if (cpuPlatform.GetCPUVendor() == "GenuineTMx86")
	{
		cpuInformation.cpuVendor = TRANSMETA;
		cpuInformation.Vendor = "Transmeta";
	}
	else if (cpuPlatform.GetCPUVendor() == "SiS SiS SiS ")
	{
		cpuInformation.cpuVendor = SIS;
		cpuInformation.Vendor = "Sis";
	}
	else if (cpuPlatform.GetCPUVendor() == "UMC UMC UMC ")
	{
		cpuInformation.cpuVendor = UMC;
		cpuInformation.Vendor = "Umc";
	}
	else if (cpuPlatform.GetCPUVendor() == "NexGenDriven")
	{
		cpuInformation.cpuVendor = NEXGEN;
		cpuInformation.Vendor = "Nexgen";
	}
	else if (cpuPlatform.GetCPUVendor() == "RiseRiseRise")
	{
		cpuInformation.cpuVendor = RISE;
		cpuInformation.Vendor = "Rise";
	}
	else if (cpuPlatform.GetCPUVendor() == "Geode by NSC")
	{
		cpuInformation.cpuVendor = NATIONAL_SEMICONDUCTOR;
		cpuInformation.Vendor = "National Semiconductor";
	}
	else
	{
		cpuInformation.cpuVendor = UNKNOWNCPUVENDOR;
		cpuInformation.Vendor = "";
	}
}

CPUINFORMATION PlatformCPUInformation::GetCPUInformation()
{
	return cpuInformation;
}

*/