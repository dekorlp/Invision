#include "precompiled.h"
#include "MemoryBlock.h"
#include "StackAllocator.h"

void StackAllocator::Init(size_t size)
{

#ifdef _DEBUG
	std::stringstream ss;
	ss << "StackAllocator::init(size = " << size << ")";
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

void* StackAllocator::Allocate(size_t blocksize, uint32 line, char* file, MemoryTracking memTracking,
	BoundsChecking boundsChecking)
{

#ifdef _DEBUG
	std::stringstream ss;
	ss << std::endl << "StackAllocator::Allocate(blocksize = " << blocksize << ", ... " << ")";
	INVISION_LOG_RAWTEXT(ss.str());
#endif



	size_t calcultedSize = MemoryBlock::CalculateSize(currentOffset, blocksize, INVISION_USE_STACKHEADER, memTracking, boundsChecking);

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

	void* p = MemoryBlock::CreateMemoryBlock(currentOffset, &currentOffset, blocksize, line, file, INVISION_USE_STACKHEADER, memTracking, boundsChecking);

	if (boundsChecking == INVISION_STANDARD_BOUNDS_CHECKING)
	{
		MemoryBlock::CheckBoundaries(p, blocksize, INVISION_USE_STACKHEADER, memTracking);
	}

	return p;
}

void StackAllocator::Deallocate(void* block)
{
#ifdef _DEBUG
	std::stringstream ss;
	ss << "StackAllocator::init(block = " << block << ")";
	INVISION_LOG_RAWTEXT(ss.str());
	ss.clear();
	ss << "arena: " << arena;
#endif

	SHeaderStack *stackheader = MemoryBlock::GetStackHeader(block);
	currentOffset = stackheader->frontOffset;

#ifdef _DEBUG
	MemoryBlock::WriteToLog("currentOffset: ", currentOffset);
#endif

	numChunks--;

}

uint32 StackAllocator::GetCountOfChunks()
{
	return numChunks;
}

void* StackAllocator::GetArena()
{
	return arena;
}

size_t StackAllocator::GetUsedMemory()
{
	return usedMemory;
}

size_t StackAllocator::GetTotalMemory()
{
	return size;
}

void StackAllocator::Clear()
{

#ifdef _DEBUG
	INVISION_LOG_RAWTEXT("");
	INVISION_LOG_RAWTEXT("StackAllocator::Clear()");
	MemoryBlock::WriteToLog("UsedMemory: ", (size_t)0);
	MemoryBlock::WriteToLog("numChunks: ", (size_t)0);
#endif

	currentOffset = arena;
	usedMemory = 0;
	numChunks = 0;
}

void StackAllocator::Destroy()
{
	delete arena;
	arena = nullptr;
}