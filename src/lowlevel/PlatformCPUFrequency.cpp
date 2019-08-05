#include "precompiled.h"
#include "PlatformCPUFrequency.h"


void PlatformCPUFrequency::StartTimingCPU()
{
	Milliseconds0 = (uint32)clock();
	uint64 _ticks0 = 0;
#ifndef _WIN64
	__asm
	{
		lea ecx, _ticks0
			mov dword ptr[ecx], 0
			mov dword ptr[ecx + 4], 0
			rdtsc
			mov[ecx], eax
			mov[ecx + 4], edx
	}
#endif //_WIN64
	Ticks0 = _ticks0;
}

void PlatformCPUFrequency::UpdateCPUTime()
{
	Milliseconds = -Milliseconds0;
	Ticks = -Ticks0;
	Milliseconds += (uint32)clock();

	uint64 _ticks = Ticks;
#ifndef _WIN64
	__asm
	{
		lea ecx, _ticks
			rdtsc
			add[ecx], eax
			adc[ecx + 4], edx
	}
#endif //_WIN64

	Ticks = _ticks;
}

int32 PlatformCPUFrequency::GetMilliseconds()
{
	return Milliseconds;
}

int64 PlatformCPUFrequency::GetTicks()
{
	return Ticks;
}

void PlatformCPUFrequency::Sleep(unsigned int mseconds)
{
	clock_t goal = mseconds + clock();
	while (goal > clock());
}

real PlatformCPUFrequency::EstimateCpuSpeed()
{
	PlatformCPUFrequency CpuFreq;
	CpuFreq.StartTimingCPU();

	do
	{
		CpuFreq.UpdateCPUTime();
		Sleep(0);

	} while (CpuFreq.GetMilliseconds() < 1000);
	return ((float)CpuFreq.GetTicks()) / ((float)CpuFreq.GetMilliseconds()) / 1000000.0f;
}
