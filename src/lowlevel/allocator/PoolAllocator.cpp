#include "precompiled.h"
#include "MemoryBlock.h"
#include "PoolAllocator.h"

void PoolAllocator::Init(size_t size, size_t chunksize, BoundsChecking boundsChecking)
{
	MemoryBlock mem;

#ifdef _DEBUG
	std::stringstream ss;
	ss << "PoolAllocator::init(size = " << size << ", chunksize = " << chunksize << ")";
	INVISION_LOG_RAWTEXT(ss.str());
	ss.clear();
	ss << "arena: " << arena;
#endif


	arena = operator new (size);
	//currentOffset = arena;
	this->size = size;
	usedMemory = 0;
	numChunks = 0;
	this->chunkSize = chunksize;
	freelist = nullptr;

	this->boundsChecking = boundsChecking;

#ifdef _DEBUG
	mem.WriteToLog("Arena: ", arena);
	mem.WriteToLog("Size: ", size);
#endif

	CreateFreeList(arena, chunksize, INVISION_DEFAULT_MEMORY_TRACKING, this->boundsChecking);

}



void PoolAllocator::CreateFreeList(void* position, size_t blocksize, MemoryTracking memTracking,
	BoundsChecking boundsChecking)
{
#ifdef _DEBUG
	std::stringstream ss;
	ss << std::endl << "PoolAllocator::CreateFreeList(position = " << position <<  ", blocksize = " << blocksize <<", ... " << ")";
	INVISION_LOG_RAWTEXT(ss.str());
	uint32 count = 0;
#endif

	MemoryBlock mem;

	void* currentPayloadPosition = nullptr;

	void* previousOffset = nullptr;
	
	size_t calcultedSize = 0;





	while (usedMemory + calcultedSize < this->size)
	{
#ifdef _DEBUG
		count++;
#endif
		calcultedSize = mem.CalculateSize(position, blocksize, INVISION_USE_POOLHEADER, memTracking, boundsChecking);

		if (previousOffset == nullptr)
		{
			// allocate top chunk
#ifdef _DEBUG
			mem.WriteToLog("Pool Block: ", count);

			mem.WriteToLog("Start: ", position);
#endif
			currentPayloadPosition = CreateFreeListBlock(position, &previousOffset, blocksize, memTracking, boundsChecking);

			freelist = currentPayloadPosition;

#ifdef _DEBUG
			mem.WriteToLog("FreeList->start: ", freelist);
			mem.WriteToLog("End: ", previousOffset);
#endif
		}
		else
		{
#ifdef _DEBUG
			mem.WriteToLog("Pool Block: ", count);

			mem.WriteToLog("Start: ", previousOffset);
#endif

			void* payload = currentPayloadPosition;
			// allocate chunks after top
			currentPayloadPosition = CreateFreeListBlock(previousOffset, &previousOffset, blocksize, memTracking, boundsChecking);
			mem.SetPoolHeader(payload, currentPayloadPosition); 
#ifdef _DEBUG
			mem.WriteToLog("previous->next: ", ((SHeaderPool*)mem.GetPoolHeader(payload))->next);
			mem.WriteToLog("End: ", previousOffset);
#endif

		}
	}
}

void* PoolAllocator::CreateFreeListBlock(void* position, void** newPosition, size_t blocksize, MemoryTracking memTracking,
	BoundsChecking boundsChecking)
{
	MemoryBlock mem;

#ifdef _DEBUG
	std::stringstream ss;
	ss << std::endl << "PoolAllocator::CreateFreeListBlock(blocksize = " << blocksize << ", ... " << ")";
	INVISION_LOG_RAWTEXT(ss.str());
#endif



	size_t calcultedSize = mem.CalculateSize(position, blocksize, INVISION_USE_POOLHEADER, memTracking, boundsChecking);

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

	void* top;

	void* p = mem.CreateMemoryBlock(position, &top, blocksize, __LINE__, __FILE__, INVISION_USE_POOLHEADER, memTracking, boundsChecking);

	if (boundsChecking == INVISION_STANDARD_BOUNDS_CHECKING)
	{
		mem.CheckBoundaries(p, blocksize, INVISION_USE_POOLHEADER, memTracking);
	}

	*newPosition = top;

	return p;
}


void* PoolAllocator::Allocate()
{

#ifdef _DEBUG
	std::stringstream ss;
	ss << "PoolAllocator::Allocatate(...)";
	INVISION_LOG_RAWTEXT(ss.str());
#endif

	MemoryBlock mem;

	if (this->freelist == nullptr)
	{
		//freelist is out of memory
#ifdef _DEBUG
		INVISION_LOG_RAWTEXT("There is not enough additional memory available!");
		throw invisionCoreOutOfMemory("There is not enough additional memory available!");
#endif

		return nullptr;
	}

	
	void* toAllocate = this->freelist;
	this->freelist = mem.GetPoolHeader(this->freelist)->next;

#ifdef _DEBUG
	mem.WriteToLog("used Block for allocation: ", toAllocate);
#endif

	numChunks++;

	// Check Bounds, when boundsChecking is activated
	if (this->boundsChecking == INVISION_STANDARD_BOUNDS_CHECKING)
	{
		mem.CheckBoundaries(toAllocate, chunkSize, INVISION_USE_POOLHEADER, INVISION_DEFAULT_MEMORY_TRACKING);
	}

	return toAllocate;
}


void PoolAllocator::Deallocate(void* block)
{
#ifdef _DEBUG
	std::stringstream ss;
	ss << "PoolAllocator::Deallocate(block = " << block <<")";
	INVISION_LOG_RAWTEXT(ss.str());
#endif

	MemoryBlock mem;


#ifdef _DEBUG
	mem.WriteToLog("deallocated Block: ", block);
#endif

	if (freelist == nullptr)
	{
		freelist = block;
	}
	else
	{
		mem.SetPoolHeader(block, freelist);
		freelist = block;
	}

	numChunks--;
}


uint32 PoolAllocator::GetCountOfChunks()
{
	return numChunks;
}

void* PoolAllocator::GetArena()
{
	return arena;
}

size_t PoolAllocator::GetUsedMemory()
{
	return usedMemory;
}

size_t PoolAllocator::GetTotalMemory()
{
	return size;
}

void PoolAllocator::Clear()
{
#ifdef _DEBUG
	std::stringstream ss;
	ss << "PoolAllocator::Clear()";
	INVISION_LOG_RAWTEXT(ss.str());
#endif

	MemoryBlock mem;

#ifdef _DEBUG
	INVISION_LOG_RAWTEXT("");
	INVISION_LOG_RAWTEXT("LinearAllocator::Clear()");
	mem.WriteToLog("UsedMemory: ", (size_t)0);
	mem.WriteToLog("numChunks: ", (size_t)0);
#endif

	//currentOffset = arena;
	usedMemory = 0;
	numChunks = 0;

	freelist == nullptr;
	CreateFreeList(arena, this->chunkSize, INVISION_DEFAULT_MEMORY_TRACKING, this->boundsChecking);
}