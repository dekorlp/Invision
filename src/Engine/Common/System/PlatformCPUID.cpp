#include "precompiled.h"
#include "PlatformCPUID.h"
#include "System\CpuId.h"
namespace Invision {
	PlatformCPU::PlatformCPU()
	{
		SetMaxFunctions();
		SetCPUVendor();
		SetCPUName();

		SetCPUInformation();

	}


	void PlatformCPU::SetMaxFunctions()
	{

		uint32 CPUInfo[4] = { 0, 0, 0, 0 };
		uint32 _highestFunction = 0;


		CPUID cpuid(0x0000000);
		memcpy(CPUInfo, &cpuid.EAX(), 4);
		memcpy(CPUInfo + 1, &cpuid.EBX(), 4);
		memcpy(CPUInfo + 2, &cpuid.EDX(), 4);
		memcpy(CPUInfo + 3, &cpuid.ECX(), 4);
		_highestFunction = CPUInfo[0];
		HighestFunction = _highestFunction;

		CPUID cpuid1(0x80000000);
		memcpy(&HighestAdvancedFunction, &cpuid1.EAX(), sizeof(HighestAdvancedFunction));

	}

	void PlatformCPU::SetCPUVendor()
	{
		char vendor[12];

		CPUID cpuid(0x00000000);
		memcpy_s(vendor, 4, &cpuid.EBX(), 4);
		memcpy_s(vendor + 4, 4, &cpuid.EDX(), 4);
		memcpy_s(vendor + 8, 4, &cpuid.ECX(), 4);

		VendorAlias = (char*)malloc(strlen(vendor) + 1);
		strcpy_s(VendorAlias, strlen(vendor) + 1, vendor);
		VendorAlias[12] = '\0';
	}

	void PlatformCPU::SetCPUName()
	{
		char name[48];

		CPUID cpuid(0x80000002);
		memcpy_s(name, 4, &cpuid.EAX(), 4);
		memcpy_s(name + 4, 4, &cpuid.EBX(), 4);
		memcpy_s(name + 8, 4, &cpuid.ECX(), 4);
		memcpy_s(name + 12, 4, &cpuid.EDX(), 4);
		CPUID cpuid1(0x80000003);
		memcpy_s(name + 16, 4, &cpuid1.EAX(), 4);
		memcpy_s(name + 20, 4, &cpuid1.EBX(), 4);
		memcpy_s(name + 24, 4, &cpuid1.ECX(), 4);
		memcpy_s(name + 28, 4, &cpuid1.EDX(), 4);
		CPUID cpuid2(0x80000004);
		memcpy_s(name + 32, 4, &cpuid2.EAX(), 4);
		memcpy_s(name + 36, 4, &cpuid2.EBX(), 4);
		memcpy_s(name + 40, 4, &cpuid2.ECX(), 4);
		memcpy_s(name + 44, 4, &cpuid2.EDX(), 4);

		CpuName = (char*)malloc(strlen(name) + 1);
		strcpy_s(CpuName, strlen(name) + 1, name);
	}


	bool PlatformCPU::SetCPUInformation()
	{
		uint32 CPUEAX = 0;
		uint32 CPUEBX = 0;
		uint32 CPUECX = 0;
		uint32 CPUEDX = 0;

		CPUID cpuid(0x00000001);
		memcpy(&CPUEAX, &cpuid.EAX(), sizeof(CPUEAX));
		memcpy(&CPUEBX, &cpuid.EBX(), sizeof(CPUEBX));
		memcpy(&CPUECX, &cpuid.ECX(), sizeof(CPUECX));
		memcpy(&CPUEDX, &cpuid.EDX(), sizeof(CPUEDX));


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

			CPUID cpuid1(0x80000001);
			memcpy(&CPUEAX, &cpuid1.EAX(), sizeof(CPUEAX));
			memcpy(&CPUEBX, &cpuid1.EBX(), sizeof(CPUEBX));
			memcpy(&CPUECX, &cpuid1.ECX(), sizeof(CPUECX));
			memcpy(&CPUEDX, &cpuid1.EDX(), sizeof(CPUEDX));

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

	uint32 PlatformCPU::GetLogicalCpuCores()
	{
		return std::thread::hardware_concurrency();
	}

	PlatformCPU::~PlatformCPU()
	{
		free(CpuName);
		free(VendorAlias);
	}

}