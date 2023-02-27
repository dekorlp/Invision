/////////////////////////////////////////////////////////////////////////////
// Name:	  PlatformCPU
// Purpose:	  low level CPUID operations
// Author:	  Dennis Koehler
// Created:	  2014-06-16
// Copyright: Copyright (c) 2015, Nano Game Foundation
//			  This unpublished source code is proprietary to Nano Game Foundation.
//			  The source code herein is considered trade secrets and confidential.
//			  Reproduction or distribution, in part or complete, is prohibited except by permission of Nano Game Foundation. 
//			  All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef PLATFORMCPUID_H
#define PLATFORMCPUID_H
namespace Invision
{
	enum INVISION_API CPUVENDOR {
		UNKNOWNCPUVENDOR = 0,
		AMD = 1,
		INTEL = 2,
		CYRIX = 3,
		CENTAUR = 4,
		TRANSMETA = 5,
		SIS = 6,
		UMC = 7,
		NEXGEN = 8,
		RISE = 9,
		NATIONAL_SEMICONDUCTOR = 10
	};

	struct INVISION_API CPUINFORMATION
	{
		char* Vendor;
		char* CpuName;
		CPUVENDOR CpuVendor;

		uint8 AdvancedFamily;
		uint8 AdvancedModel;
		uint8 Family;
		uint8 Model;
		uint8 Stepping;
		uint8 BrandID;
		uint8 CFlushSize;
		uint8 CountLogicalProcessors;
		uint8 APICID;

		struct
		{
			uint8 Type;
			bool PSN;
			bool DTES;
			bool ACPI;
			bool SS;
			bool TM1;
			bool IA64;
			bool PBE;
		} Intel;

		struct
		{
			bool SYSCALL;
			bool XD;
			bool MMXEXT;
			bool FFXSR;
			bool LM;
			bool DDNOWEXT;
			bool DDNOW;
		} Amd;

		bool FPU;
		bool VME;
		bool DE;
		bool PSE;
		bool TSC;
		bool MSR;
		bool PAE;
		bool MCE;
		bool CX8;
		bool APIC;
		bool SEP;
		bool MTRR;
		bool PGE;
		bool MCA;
		bool CMOV;
		bool PAT;
		bool PSE36;
		bool CLFL;
		bool MMX;
		bool FXSR;
		bool SSE;
		bool SSE2;
		bool HTT;
	};

	typedef CPUINFORMATION CpuInformation;

	class INVISION_API PlatformCPU
	{
	private:
		CPUINFORMATION CpuInformation;
		uint32 HighestFunction = 0;
		uint32 HighestAdvancedFunction = 0;
		char *VendorAlias;
		char *CpuName;

		void  SetMaxFunctions();
		void  SetCPUVendor();
		void  SetCPUName();
		bool  SetCPUInformation();

	public:
		PlatformCPU();
		const char*  GetCPUVendor();
		const char*  GetCpuName();
		uint32 GetHighestFunction();
		uint32 GetHighestAdvancedFunction();
		uint32 GetLogicalCpuCores();

		CPUINFORMATION GetCPUInformation();

		~PlatformCPU();
	};
}
#endif //PLATFORMCPUID_H