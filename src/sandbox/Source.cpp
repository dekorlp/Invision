#include "lowlevel.h"
#include "inMath.h"
#include "Log.h"
#include "allocator\MemoryBlock.h"
#include "allocator\LinearAllocator.h"
#include "allocator\StackAllocator.h"
#include <iostream>

using namespace std;

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

/*void testAllocators()
{
	std::cout << "INT: " << sizeof(int) << std::endl;
	std::cout << "CHAR: " << sizeof(char) << std::endl;
	std::cout << "CHAR*: " << sizeof(char*) << std::endl;
	std::cout << "SIZE_T: " << sizeof(size_t) << std::endl;
	std::cout << "UNSIGNED INT: " << sizeof(unsigned int) << std::endl;

	typedef Allocator<StackAllocator, StandardBoundsChecking, AdvancedMemoryTracking> TestAllocator;

	//TestAllocator Loca(1024*1024); // 1MB
	TestAllocator Loca(sizeof(int), 4);
	int *test = (int*)Loca.allocate(sizeof(int), __LINE__, __FILE__);
	*test = 2147483647;
	int *test1 = (int*)Loca.allocate(sizeof(int), __LINE__, __FILE__);
	*test1 = 14;
	int *test2 = (int*)Loca.allocate(sizeof(int), __LINE__, __FILE__);
	*test2 = 15;
	int *test3 = (int*)Loca.allocate(sizeof(int), __LINE__, __FILE__);
	*test3 = 16;
	Loca.deallocate(test3, __LINE__, __FILE__);
	Loca.deallocate(test2, __LINE__, __FILE__);
	int *test4 = (int*)Loca.allocate(sizeof(int), __LINE__, __FILE__);
	*test4 = 17;
	int *test5 = (int*)Loca.allocate(sizeof(int), __LINE__, __FILE__);
	*test5 = 18;
	std::cout << *test5;

}*/

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

void testAllocatorsStack()
{
	//1024 * 1024
	//void* arena = ::operator new (1);
	uint32 length = 1024;
	void* arena = malloc(length);
	Log log("../../../logs/AllocationLog.txt");
	Log::SetLogger(&log);
	MemoryBlock memBlock;
	
	void* top;
	
	int* test6 = (int*)memBlock.CreateMemoryBlock(arena, &top, sizeof(int), __LINE__, __FILE__, INVISION_USE_STACKHEADER, INVISION_ADVANCED_MEMORY_TRACKING, INVISION_STANDARD_BOUNDS_CHECKING);
	*test6 = 0x2BEC;
	SHeaderStack *header = memBlock.GetStackHeader(test6);
	SMemoryTracking* tracking = memBlock.GetTrackingHeader(test6, INVISION_USE_STACKHEADER);
	bool isBoundaries = memBlock.CheckBoundaries(test6, sizeof(int), INVISION_USE_STACKHEADER, INVISION_ADVANCED_MEMORY_TRACKING);
	uint32 size = memBlock.CalculateSize(arena, sizeof(int), INVISION_USE_STACKHEADER, INVISION_ADVANCED_MEMORY_TRACKING, INVISION_STANDARD_BOUNDS_CHECKING);
	// Ausgabe Size
	stringstream ss1;
	ss1 << "calculateSize(): " << size << std::endl;
	INVISION_LOG_RAWTEXT(ss1.str());

	// Ausgabe Top
	stringstream ss2;
	ss2 << "Top: " << top << std::endl;
	INVISION_LOG_RAWTEXT(ss2.str());

}

void testAllocatorPool()
{
	//1024 * 1024
	//void* arena = ::operator new (1);
	uint32 length = 1024;
	void* arena = malloc(length);
	Log log("../../../logs/AllocationLog.txt");
	Log::SetLogger(&log);
	MemoryBlock memBlock;

	void* top;

	int* test6 = (int*)memBlock.CreateMemoryBlock(arena, &top, sizeof(int), __LINE__, __FILE__, INVISION_USE_POOLHEADER, INVISION_ADVANCED_MEMORY_TRACKING, INVISION_STANDARD_BOUNDS_CHECKING);
	*test6 = 0x2BEC;

	INVISION_LOG_RAWTEXT("----------Before SetPoolHeader()--------");
	SHeaderPool *header1 = memBlock.GetPoolHeader(test6);
	memBlock.SetPoolHeader(test6, 0xFFFFFFFF);
	INVISION_LOG_RAWTEXT("----------After SetPoolHeader()-------");
	SHeaderPool *header2 = memBlock.GetPoolHeader(test6);
	SMemoryTracking* tracking = memBlock.GetTrackingHeader(test6, INVISION_USE_POOLHEADER);
	bool isBoundaries = memBlock.CheckBoundaries(test6, sizeof(int), INVISION_USE_POOLHEADER, INVISION_ADVANCED_MEMORY_TRACKING);
	uint32 size = memBlock.CalculateSize(arena, sizeof(int), INVISION_USE_POOLHEADER, INVISION_ADVANCED_MEMORY_TRACKING, INVISION_STANDARD_BOUNDS_CHECKING);

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
	Log log("../../../logs/AllocationLog.txt");
	Log::SetLogger(&log);


	LinearAllocator alloc;
	alloc.Init(1024);
	uint32* LinA1 = (uint32*) alloc.Allocate(sizeof(int), __LINE__, __FILE__, INVISION_ADVANCED_MEMORY_TRACKING, INVISION_STANDARD_BOUNDS_CHECKING);
	*LinA1 = 44;
	uint32* LinA2 = (uint32*) alloc.Allocate(sizeof(int), __LINE__, __FILE__, INVISION_ADVANCED_MEMORY_TRACKING, INVISION_STANDARD_BOUNDS_CHECKING);
	*LinA2 = 125;
	uint32* LinA3 = (uint32*) alloc.Allocate(sizeof(int), __LINE__, __FILE__, INVISION_ADVANCED_MEMORY_TRACKING, INVISION_STANDARD_BOUNDS_CHECKING);
	*LinA3 = 128;
	alloc.Clear();
	uint32* LinA4 = (uint32*)alloc.Allocate(sizeof(int), __LINE__, __FILE__, INVISION_ADVANCED_MEMORY_TRACKING, INVISION_STANDARD_BOUNDS_CHECKING);
	*LinA1 = 1028;
}

void testStackAllocator()
{
	Log log("../../../logs/AllocationLog.txt");
	Log::SetLogger(&log);

	StackAllocator alloc;
	alloc.Init(1024);
	uint32* LinA1 = (uint32*)alloc.Allocate(sizeof(int), __LINE__, __FILE__, INVISION_ADVANCED_MEMORY_TRACKING, INVISION_STANDARD_BOUNDS_CHECKING);
	*LinA1 = 44;
	uint32* LinA2 = (uint32*)alloc.Allocate(sizeof(int), __LINE__, __FILE__, INVISION_ADVANCED_MEMORY_TRACKING, INVISION_STANDARD_BOUNDS_CHECKING);
	*LinA2 = 125;
	uint32* LinA3 = (uint32*)alloc.Allocate(sizeof(int), __LINE__, __FILE__, INVISION_ADVANCED_MEMORY_TRACKING, INVISION_STANDARD_BOUNDS_CHECKING);
	*LinA3 = 128;
	alloc.Deallocate(LinA2);
	uint32* LinA4 = (uint32*)alloc.Allocate(sizeof(int), __LINE__, __FILE__, INVISION_ADVANCED_MEMORY_TRACKING, INVISION_STANDARD_BOUNDS_CHECKING);
	*LinA4 = 1028;
	uint32* LinA5 = (uint32*)alloc.Allocate(sizeof(int), __LINE__, __FILE__, INVISION_ADVANCED_MEMORY_TRACKING, INVISION_STANDARD_BOUNDS_CHECKING);
	*LinA5 = 2048;
	alloc.Clear();
	uint32* LinA6 = (uint32*)alloc.Allocate(sizeof(int), __LINE__, __FILE__, INVISION_ADVANCED_MEMORY_TRACKING, INVISION_STANDARD_BOUNDS_CHECKING);
	*LinA6 = 4096;
}


int main()
{
	//testVector();
	//testAllocators();
	//testLog();

	//testAllocatorsStack();
	//testAllocatorPool();
	//testLinearAllocator();
	testStackAllocator();
	
	return 0;
}