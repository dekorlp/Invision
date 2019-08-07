#include "lowlevel.h"
#include "inMath.h"
#include "Log.h"
#include "allocator\MemoryBlock.h"
#include "allocator\LinearAllocator.h"
#include "allocator\StackAllocator.h"
#include "allocator\PoolAllocator.h"
#include "PlatformCPUID.h"
#include "PlatformCPUFrequency.h"
#include "VulkanCanvas.h"
#include <iostream>



using namespace std;

void testCPUInfo()
{
	PlatformCPU platformCpu;
	PlatformCPUFrequency cpufreq;

	const char* name = platformCpu.GetCpuName();
	std::cout << "Name: " << name << std::endl;
	std::cout << "Count V-Cores: " << platformCpu.GetLogicalCpuCores() << std::endl;
	std::cout << "Frequency: " << cpufreq.EstimateCpuSpeed() << std::endl;
}

void testVector()
{
	Vector3 vec1(1, 4, 2);
	//Vector3 vec2(4, 3, 1);
	Vector3 vec2(2, 4, 2);
	//Vector3 res = vec1 + vec2;
	//Vector3 res = vec2 - vec1;
	//res = vec2 + vec1;
	bool b = vec1 == vec2;
}

void testLog()
{
	Log log("../../../logs/log.txt");
	log.RawText(std::string("HalloWelt").append("RAW"));
	log.Info(std::string("HalloWelt").append("INFO"));
	log.Error(std::string("HalloWelt").append("ERROR"));
	log.Warning(std::string("HalloWelt").append("WARNING"));
	//Log::m_pThis = &log;
	Log::SetLogger(&log);
	INVISION_LOG_INFO(std::string("HalloWelt").append("INFO"));
}

void testAllocatorBlockStack()
{
	//1024 * 1024
	//void* arena = ::operator new (1);
	uint32 length = 1024;
	void* arena = malloc(length);
	Log log("../../../logs/AllocationLog.txt");
	Log::SetLogger(&log);
	
	void* top;
	
	int* test6 = (int*)MemoryBlock::CreateMemoryBlock(arena, &top, sizeof(int), __LINE__, __FILE__, INVISION_USE_STACKHEADER, INVISION_ADVANCED_MEMORY_TRACKING, INVISION_STANDARD_BOUNDS_CHECKING);
	*test6 = 0x2BEC;
	SHeaderStack *header = MemoryBlock::GetStackHeader(test6);
	SMemoryTracking* tracking = MemoryBlock::GetTrackingHeader(test6, INVISION_USE_STACKHEADER);
	bool isBoundaries = MemoryBlock::CheckBoundaries(test6, sizeof(int), INVISION_USE_STACKHEADER, INVISION_ADVANCED_MEMORY_TRACKING);
	size_t size = MemoryBlock::CalculateSize(arena, sizeof(int), INVISION_USE_STACKHEADER, INVISION_ADVANCED_MEMORY_TRACKING, INVISION_STANDARD_BOUNDS_CHECKING);
	// Ausgabe Size
	stringstream ss1;
	ss1 << "calculateSize(): " << size << std::endl;
	INVISION_LOG_RAWTEXT(ss1.str());

	// Ausgabe Top
	stringstream ss2;
	ss2 << "Top: " << top << std::endl;
	INVISION_LOG_RAWTEXT(ss2.str());

}

void testAllocatorBlockPool()
{
	//1024 * 1024
	//void* arena = ::operator new (1);
	uint32 length = 1024;
	void* arena = malloc(length);
	Log log("../../../logs/AllocationLog.txt");
	Log::SetLogger(&log);

	void* top;

	int* test6 = (int*)MemoryBlock::CreateMemoryBlock(arena, &top, sizeof(int), __LINE__, __FILE__, INVISION_USE_POOLHEADER, INVISION_ADVANCED_MEMORY_TRACKING, INVISION_STANDARD_BOUNDS_CHECKING);
	*test6 = 0x2BEC;

	INVISION_LOG_RAWTEXT("----------Before SetPoolHeader()--------");
	SHeaderPool *header1 = MemoryBlock::GetPoolHeader(test6);
	//memBlock.SetPoolHeader(test6, (void*)0xFFFFFFFF);
	INVISION_LOG_RAWTEXT("----------After SetPoolHeader()-------");
	SHeaderPool *header2 = MemoryBlock::GetPoolHeader(test6);
	SMemoryTracking* tracking = MemoryBlock::GetTrackingHeader(test6, INVISION_USE_POOLHEADER);
	bool isBoundaries = MemoryBlock::CheckBoundaries(test6, sizeof(int), INVISION_USE_POOLHEADER, INVISION_ADVANCED_MEMORY_TRACKING);
	size_t size = MemoryBlock::CalculateSize(arena, sizeof(int), INVISION_USE_POOLHEADER, INVISION_ADVANCED_MEMORY_TRACKING, INVISION_STANDARD_BOUNDS_CHECKING);

	// Ausgabe Size
	stringstream ss1;
	ss1 << "calculateSize(): " << size << std::endl;
	INVISION_LOG_RAWTEXT(ss1.str());

	// Ausgabe Top
	stringstream ss;
	ss << "Top: " << top << std::endl;
	INVISION_LOG_RAWTEXT(ss.str());
}

void testLinearAllocator()
{
	Log log("../../../logs/LinearAllocationLog.txt");
	Log::SetLogger(&log);


	LinearAllocator alloc;
	alloc.Init(1024);
	uint32* LinA1 = (uint32*) alloc.Allocate(sizeof(int));
	*LinA1 = 44;
	uint32* LinA2 = (uint32*) alloc.Allocate(sizeof(int));
	*LinA2 = 125;
	uint32* LinA3 = (uint32*) alloc.Allocate(sizeof(int));
	*LinA3 = 128;
	alloc.Clear();
	uint32* LinA4 = (uint32*)alloc.Allocate(sizeof(int));
	*LinA4 = 1028;

	alloc.Destroy();
}

void testStackAllocator()
{
	Log log("../../../logs/StackAllocationLog.txt");
	Log::SetLogger(&log);

	StackAllocator alloc;
	alloc.Init(1024);
	uint32* LinA1 = (uint32*)alloc.Allocate(sizeof(int));
	*LinA1 = 44;
	uint32* LinA2 = (uint32*)alloc.Allocate(sizeof(int));
	*LinA2 = 125;
	uint32* LinA3 = (uint32*)alloc.Allocate(sizeof(int));
	*LinA3 = 128;
	alloc.Deallocate(LinA2);
	uint32* LinA4 = (uint32*)alloc.Allocate(sizeof(int));
	*LinA4 = 1028;
	uint32* LinA5 = (uint32*)alloc.Allocate(sizeof(int));
	*LinA5 = 2048;
	alloc.Clear();
	uint32* LinA6 = (uint32*)alloc.Allocate(sizeof(int));
	*LinA6 = 4096;

	alloc.Destroy();
}

void testPoolAllocator()
{
	Log log("../../../logs/PoolAllocationLog.txt");
	Log::SetLogger(&log);

	PoolAllocator alloc;
	alloc.Init(192, sizeof(int));
	uint32* Pol1 = (uint32*) alloc.Allocate();
	*Pol1 = 1024;
	uint32* Pol2 = (uint32*)alloc.Allocate();
	*Pol2 = 2048;
	uint32* Pol3 = (uint32*)alloc.Allocate();
	*Pol3 = 3000;
	uint32* Pol4 = (uint32*)alloc.Allocate();
	*Pol4 = 4000;
	uint32* Pol5 = (uint32*)alloc.Allocate();
	*Pol5 = 5000;
	uint32* Pol6 = (uint32*)alloc.Allocate();
	*Pol6 = 7000;
	alloc.Deallocate(Pol2);
	alloc.Deallocate(Pol3);
	uint32* Pol7 = (uint32*)alloc.Allocate();
	*Pol7 = 8000;
	uint32* Pol8 = (uint32*)alloc.Allocate();
	*Pol8 = 9000;

	alloc.Destroy();
}

int main()
{

	VulkanCanvas vkCanvas;

	//CPUID cpuid(0x80000004);

	//std::string vendor;
	//vendor += std::string((const char *)&cpuid.EBX(), 4);
	//vendor += std::string((const char *)&cpuid.EDX(), 4);
	//vendor += std::string((const char *)&cpuid.ECX(), 4);

	//cout << "CPU vendor = " << vendor << endl;



	//testVector();
	//testAllocators();
	//testLog();

	//testAllocatorBlockStack();
	//testAllocatorBlockPool();
	testCPUInfo();
	testLinearAllocator();
	testStackAllocator();
	testPoolAllocator();
	
	return 0;
}