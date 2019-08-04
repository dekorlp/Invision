#include "precompiled.h"
#include "MemoryBlock.h"
#include "LinearAllocator.h"

void LinearAllocator::Init(size_t size)
{

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
	MemoryBlock::WriteToLog("Arena: ", arena);
	MemoryBlock::WriteToLog("Size: ", size);
#endif
}

void* LinearAllocator::Allocate(size_t blocksize, uint32 line, char* file, MemoryTracking memTracking,
	BoundsChecking boundsChecking)
{
	

#ifdef _DEBUG
	std::stringstream ss;
	ss << std::endl << "LinearAllocator::Allocate(blocksize = " << blocksize << ", ... " << ")";
	INVISION_LOG_RAWTEXT(ss.str());
#endif



	size_t calcultedSize = MemoryBlock::CalculateSize(currentOffset, blocksize, INVISION_USE_NO_HEADER, memTracking, boundsChecking);

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
	MemoryBlock::WriteToLog("usedMemory: ", usedMemory);
	MemoryBlock::WriteToLog("Size Of Chunk: ", calcultedSize);
	MemoryBlock::WriteToLog("numChunks: ", numChunks);
#endif

	void* p = MemoryBlock::CreateMemoryBlock(currentOffset, &currentOffset, blocksize, line, file, INVISION_USE_NO_HEADER, memTracking, boundsChecking);

	if (boundsChecking == INVISION_STANDARD_BOUNDS_CHECKING)
	{
		MemoryBlock::CheckBoundaries(p, blocksize, INVISION_USE_NO_HEADER, memTracking);
	}

	return p;
}

uint32 LinearAllocator::GetCountOfChunks()
{
	return numChunks;
}

void* LinearAllocator::GetArena()
{
	return arena;
}

size_t LinearAllocator::GetUsedMemory()
{
	return usedMemory;
}

size_t LinearAllocator::GetTotalMemory()
{
	return size;
}

void LinearAllocator::Clear()
{

#ifdef _DEBUG
	INVISION_LOG_RAWTEXT("");
	INVISION_LOG_RAWTEXT("LinearAllocator::Clear()");
	MemoryBlock::WriteToLog("UsedMemory: ", (size_t)0);
	MemoryBlock::WriteToLog("numChunks: ", (size_t)0);
#endif

	currentOffset = arena;
	usedMemory = 0;
	numChunks = 0;
}

void LinearAllocator::Destroy()
{
	delete arena;
	arena = nullptr;
}