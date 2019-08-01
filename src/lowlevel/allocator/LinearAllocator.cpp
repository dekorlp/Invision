#include "precompiled.h"
#include "MemoryBlock.h"
#include "LinearAllocator.h"

void LinearAllocator::init(size_t size)
{
	MemoryBlock mem;

#ifdef _DEBUG
	std::stringstream ss;
	ss << "LinearAllocator::init(size = " << size << ")";
	INVISION_LOG_RAWTEXT(ss.str());
	ss.clear();
	ss << "arena: " << arena;
#endif

	
	arena = operator new (size);
	currentOffset = arena;
	this->size = size;
	usedMemory = 0;
	numChunks = 0;

#ifdef _DEBUG
	mem.WriteToLog("Arena: ", arena);
	mem.WriteToLog("Size: ", size);
#endif
}

void* LinearAllocator::Allocate(size_t blocksize, uint32 line, char* file, MemoryTracking memTracking,
	BoundsChecking boundsChecking)
{
	MemoryBlock mem;

#ifdef _DEBUG
	std::stringstream ss;
	ss << std::endl << "LinearAllocator::Allocate(blocksize = " << blocksize << ", ... " << ")";
	INVISION_LOG_RAWTEXT(ss.str());
#endif



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
	

#ifdef _DEBUG
	mem.WriteToLog("usedMemory: ", usedMemory);
	mem.WriteToLog("Size Of Chunk: ", calcultedSize);
	mem.WriteToLog("numChunks: ", numChunks);
#endif

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

size_t LinearAllocator::getUsedMemory()
{
	return usedMemory;
}

size_t LinearAllocator::getTotalMemory()
{
	return size;
}

void LinearAllocator::clear()
{
	MemoryBlock mem;

#ifdef _DEBUG
	INVISION_LOG_RAWTEXT("");
	INVISION_LOG_RAWTEXT("LinearAllocator::Clear()");
	mem.WriteToLog("UsedMemory: ", (size_t)0);
	mem.WriteToLog("numChunks: ", (size_t)0);
#endif

	currentOffset = arena;
	usedMemory = 0;
	numChunks = 0;
}