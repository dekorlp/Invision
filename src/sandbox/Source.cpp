﻿#include "lowlevel.h"
#include "inMath.h"
#include "Log.h"
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

void testAllocators()
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

int main()
{
	//testVector();
	//testAllocators();
	testLog();
	
	return 0;
}