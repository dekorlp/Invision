#include "precompiled.h"
#include "MemoryBlock.h"
#include "PoolAllocator.h"
namespace Invision {
	PoolAllocator::PoolAllocator()
	{
		arena = nullptr;
		size = 0;
		usedMemory = 0;
		numChunks = 0;
		chunkSize = 0;
		freelist = nullptr;
	}

	PoolAllocator::PoolAllocator(size_t size, size_t chunksize, BoundsChecking boundsChecking)
	{
		Init(size, chunksize, boundsChecking);
	}

	void PoolAllocator::InitBlocks(size_t count, size_t chunksize, BoundsChecking boundsChecking, MemoryTracking memoryTracking)
	{
		size_t sizeLayout = MemoryBlock::CalculateLayoutSize(INVISION_USE_POOLHEADER, memoryTracking, boundsChecking);
		Init(count * (chunksize + sizeLayout), chunksize, boundsChecking, memoryTracking);
	}

	void PoolAllocator::Init(size_t size, size_t chunksize, BoundsChecking boundsChecking, MemoryTracking memoryTracking)
	{

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
		mTracking = memoryTracking;

#ifdef _DEBUG
		Log::GetLogger()->WriteToLog("Arena: ", arena);
		Log::GetLogger()->WriteToLog("Size: ", size);
#endif

		CreateFreeList(arena, chunksize, memoryTracking, this->boundsChecking);

	}



	void PoolAllocator::CreateFreeList(void* position, size_t blocksize, MemoryTracking memTracking,
		BoundsChecking boundsChecking)
	{
#ifdef _DEBUG
		std::stringstream ss;
		ss << std::endl << "PoolAllocator::CreateFreeList(position = " << position << ", blocksize = " << blocksize << ", ... " << ")";
		INVISION_LOG_RAWTEXT(ss.str());
		uint32 count = 0;
#endif

		void* currentPayloadPosition = nullptr;

		void* previousOffset = nullptr;

		size_t calcultedSize = 0;




		while (usedMemory + calcultedSize <= this->size)
		{
#ifdef _DEBUG
			count++;
#endif
			calcultedSize = MemoryBlock::CalculateSize(position, blocksize, INVISION_USE_POOLHEADER, memTracking, boundsChecking);

			if (previousOffset == nullptr)
			{
				// allocate top chunk
#ifdef _DEBUG
				Log::GetLogger()->WriteToLog("Pool Block: ", count);

				Log::GetLogger()->WriteToLog("Start: ", position);
#endif
				currentPayloadPosition = CreateFreeListBlock(position, &previousOffset, blocksize, memTracking, boundsChecking);

				freelist = currentPayloadPosition;

#ifdef _DEBUG
				Log::GetLogger()->WriteToLog("FreeList->start: ", freelist);
				Log::GetLogger()->WriteToLog("End: ", previousOffset);
#endif
			}
			else
			{
#ifdef _DEBUG
				Log::GetLogger()->WriteToLog("Pool Block: ", count);

				Log::GetLogger()->WriteToLog("Start: ", previousOffset);
#endif

				void* payload = currentPayloadPosition;
				// allocate chunks after top
				currentPayloadPosition = CreateFreeListBlock(previousOffset, &previousOffset, blocksize, memTracking, boundsChecking);
				MemoryBlock::SetPoolHeader(payload, currentPayloadPosition);
#ifdef _DEBUG
				Log::GetLogger()->WriteToLog("previous->next: ", ((SHeaderPool*)MemoryBlock::GetPoolHeader(payload))->next);
				Log::GetLogger()->WriteToLog("End: ", previousOffset);
#endif

			}
		}
	}

	void* PoolAllocator::CreateFreeListBlock(void* position, void** newPosition, size_t blocksize, MemoryTracking memTracking,
		BoundsChecking boundsChecking)
	{

#ifdef _DEBUG
		std::stringstream ss;
		ss << std::endl << "PoolAllocator::CreateFreeListBlock(blocksize = " << blocksize << ", ... " << ")";
		INVISION_LOG_RAWTEXT(ss.str());
#endif



		size_t calcultedSize = MemoryBlock::CalculateSize(position, blocksize, INVISION_USE_POOLHEADER, memTracking, boundsChecking);

		if (usedMemory + calcultedSize > size)
		{
#ifdef _DEBUG
			throw InvisionOutOfMemory("There is not enough additional memory available!");
#endif
			INVISION_LOG_RAWTEXT("There is not enough additional memory available!");
			return nullptr;
		}

		usedMemory += calcultedSize;
		numChunks++;


#ifdef _DEBUG
		Log::GetLogger()->WriteToLog("usedMemory: ", usedMemory);
		Log::GetLogger()->WriteToLog("Size Of Chunk: ", calcultedSize);
		Log::GetLogger()->WriteToLog("numChunks: ", numChunks);
#endif

		void* top;

		void* p = MemoryBlock::CreateMemoryBlock(position, &top, blocksize, __LINE__, __FILE__, INVISION_USE_POOLHEADER, memTracking, boundsChecking);

		if (boundsChecking == INVISION_STANDARD_BOUNDS_CHECKING)
		{
			MemoryBlock::CheckBoundaries(p, blocksize, INVISION_USE_POOLHEADER, memTracking);
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


		if (this->freelist == nullptr)
		{
			//freelist is out of memory
#ifdef _DEBUG
			INVISION_LOG_RAWTEXT("There is not enough additional memory available!");
			throw InvisionOutOfMemory("There is not enough additional memory available!");
#endif

			return nullptr;
		}


		void* toAllocate = this->freelist;
		this->freelist = MemoryBlock::GetPoolHeader(this->freelist)->next;

#ifdef _DEBUG
		Log::GetLogger()->WriteToLog("used Block for allocation: ", toAllocate);
#endif

		numChunks++;

		// Check Bounds, when boundsChecking is activated
		if (this->boundsChecking == INVISION_STANDARD_BOUNDS_CHECKING)
		{
			MemoryBlock::CheckBoundaries(toAllocate, chunkSize, INVISION_USE_POOLHEADER, mTracking);
		}

		return toAllocate;
	}


	void PoolAllocator::Deallocate(void* block)
	{
#ifdef _DEBUG
		std::stringstream ss;
		ss << "PoolAllocator::Deallocate(block = " << block << ")";
		INVISION_LOG_RAWTEXT(ss.str());
#endif


#ifdef _DEBUG
		Log::GetLogger()->WriteToLog("deallocated Block: ", block);
#endif

		if (freelist == nullptr)
		{
			freelist = block;
		}
		else
		{
			MemoryBlock::SetPoolHeader(block, freelist);
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

	size_t PoolAllocator::GetLayoutSize()
	{
		return MemoryBlock::CalculateLayoutSize(INVISION_USE_POOLHEADER, mTracking, boundsChecking);
	}

	void PoolAllocator::Clear()
	{
#ifdef _DEBUG
		std::stringstream ss;
		ss << "PoolAllocator::Clear()";
		INVISION_LOG_RAWTEXT(ss.str());
#endif

#ifdef _DEBUG
		INVISION_LOG_RAWTEXT("");
		INVISION_LOG_RAWTEXT("LinearAllocator::Clear()");
		Log::GetLogger()->WriteToLog("UsedMemory: ", (size_t)0);
		Log::GetLogger()->WriteToLog("numChunks: ", (size_t)0);
#endif

		//currentOffset = arena;
		usedMemory = 0;
		numChunks = 0;

		freelist = nullptr;
		CreateFreeList(arena, this->chunkSize, mTracking, this->boundsChecking);
	}

	void PoolAllocator::Destroy()
	{
		delete arena;
		arena = nullptr;
	}

	PoolAllocator::~PoolAllocator()
	{
		Destroy();
	}
}