#include "precompiled.h"
#include "MemoryBlock.h"
#include "LinearAllocator.h"

void LinearAllocator::init(uint32 size)
{
	arena = operator new (size);
	currentOffset = arena;
	this->size = size;
	usedMemory = 0;
	numChunks = 0;
}

void* LinearAllocator::Allocate(uint32 blocksize, uint32 line, char* file, MemoryTracking memTracking,
	BoundsChecking boundsChecking)
{
	

	MemoryBlock mem;

	uint32 calcultedSize = mem.CalculateSize(currentOffset, blocksize, INVISION_USE_NO_HEADER, memTracking, boundsChecking);

	if (usedMemory + calcultedSize > size)
	{
#ifdef _DEBUG
		throw invisionCoreOutOfMemory("There is not enough additional memory available!");
#endif
		INVISION_LOG_RAWTEXT("There is not enough additional memory available!");
		return nullptr;
	}

	usedMemory += calcultedSize;
	numChunks++;

	void* p = mem.CreateMemoryBlock(currentOffset, &currentOffset, blocksize, line, file, INVISION_USE_NO_HEADER, memTracking, boundsChecking);
	mem.CheckBoundaries(p, blocksize, INVISION_USE_NO_HEADER, memTracking);
	return p;
}

uint32 LinearAllocator::getCountOfChunks()
{
	return numChunks;
}

void* LinearAllocator::getArena()
{
	return arena;
}

uint32 LinearAllocator::getUsedMemory()
{
	return usedMemory;
}

uint32 LinearAllocator::getTotalMemory()
{
	return size;
}

void LinearAllocator::clear()
{
	currentOffset = arena;
	usedMemory = 0;
	numChunks = 0;
}