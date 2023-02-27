#include "precompiled.h"
#include "PlatformCPUFrequency.h"




namespace Invision {
#ifdef _WIN32

	DWORD PlatformCPUFrequency::ReadCPUSpeedFromRegistry()
	{
		//CString sMHz;
		char Buffer[_MAX_PATH];
		DWORD BufSize = _MAX_PATH;
		DWORD dwMHz = _MAX_PATH;
		HKEY hKey;

		// open the key where the proc speed is hidden:
		long lError = RegOpenKeyEx(HKEY_LOCAL_MACHINE,
			"HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0",
			0,
			KEY_READ,
			&hKey);

		if (lError != ERROR_SUCCESS)
		{// if the key is not found, tell the user why:
			FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM,
				NULL,
				lError,
				0,
				Buffer,
				_MAX_PATH,
				0);
			//AfxMessageBox(Buffer);
			return 0;
		}

		// query the key:
		RegQueryValueEx(hKey, "~MHz", NULL, NULL, (LPBYTE)&dwMHz, &BufSize);

		// convert the DWORD to a CString:
		//sMHz.Format("%i", dwMHz);

		return dwMHz;
	}
#elif 
	void PlatformCPUFrequency::StartTimingCPU()
	{
		Milliseconds0 = (uint32)clock();
		uint64 _ticks0 = 0;

		__asm
		{
			lea ecx, _ticks0
			mov dword ptr[ecx], 0
			mov dword ptr[ecx + 4], 0
			rdtsc
			mov[ecx], eax
			mov[ecx + 4], edx
		}

		Ticks0 = _ticks0;
	}

	void PlatformCPUFrequency::UpdateCPUTime()
	{
		Milliseconds = -Milliseconds0;
		Ticks = -Ticks0;
		Milliseconds += (uint32)clock();

		uint64 _ticks = Ticks;

		__asm
		{
			lea ecx, _ticks
			rdtsc
			add[ecx], eax
			adc[ecx + 4], edx
		}


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
#endif

	real PlatformCPUFrequency::EstimateCpuSpeed()
	{
#ifdef _WIN32
		return ReadCPUSpeedFromRegistry();
#elif
		PlatformCPUFrequency CpuFreq;
		CpuFreq.StartTimingCPU();

		do
		{
			CpuFreq.UpdateCPUTime();
			Sleep(0);

		} while (CpuFreq.GetMilliseconds() < 1000);
		return ((float)CpuFreq.GetTicks()) / ((float)CpuFreq.GetMilliseconds()) / 1000000.0f;
#endif
	}

}