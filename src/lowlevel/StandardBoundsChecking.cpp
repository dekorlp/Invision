#include "precompiled.h"
#include "StandardBoundsChecking.h"
#include "PointerUtil.h"

inline void StandardBoundsChecking::observeFront(void *memory, unsigned int headerSize)
{

	unsigned int headersize = headerSize;
	
	unsigned int *bound = (unsigned int*)PointerUtil::subtract(memory, headersize);
	*bound = 0xFAFFB;
}

inline void StandardBoundsChecking::observeBack(void *memory)
{
	//int headersize = sizeof(int);

	unsigned int *bound = (unsigned int*)memory;
	*bound = 0xFAFFB;
}

inline void StandardBoundsChecking::checkFront(void *memory, unsigned int offset, unsigned int headerSize)
{
	unsigned int headersize = headerSize;

	unsigned int *bound = (unsigned int*)(PointerUtil::subtract(memory, headersize + offset + FRONT_SIZE));

	if (*bound != 0xFAFFB)
	{
		throw invisionCoreOutOfMemory("Memory is corrupted!");
	}

}

inline void StandardBoundsChecking::checkBack(void *memory)
{
	unsigned int *bound = (unsigned int*)(PointerUtil::add(memory, BACK_SIZE));

	if (*bound != 0xFAFFB)
	{
		throw invisionCoreOutOfMemory("Memory is corrupted!");
	}

}