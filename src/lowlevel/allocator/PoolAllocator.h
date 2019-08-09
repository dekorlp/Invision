/////////////////////////////////////////////////////////////////////////////
// Name:	  PoolAllocator
// Purpose:	  allocator subComponent allocator who allows pool based Allocations
// Author:	  Dennis Koehler
// Created:	  2015-06-12
// Copyright: Copyright (c) 2015, Nano Game Foundation
//			  This unpublished source code is proprietary to Nano Game Foundation.
//			  The source code herein is considered trade secrets and confidential.
//			  Reproduction or distribution, in part or complete, is prohibited except by permission of Nano Game Foundation. 
//			  All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#ifndef POOLALLOCATOR_H
#define POOLALLOCATOR_H

namespace Invision
{
	class INVISION_API PoolAllocator
	{
	public:
		explicit PoolAllocator();
		explicit PoolAllocator(size_t size, size_t chunksize, BoundsChecking boundsChecking = INVISION_NO_BOUNDS_CHECKING);

		PoolAllocator(PoolAllocator&&) = delete;
		PoolAllocator& operator==(PoolAllocator&&) = delete;
		PoolAllocator(const PoolAllocator&) = delete;
		PoolAllocator& operator=(const PoolAllocator&) = delete;

		void Init(size_t size, size_t chunksize, BoundsChecking boundsChecking = INVISION_NO_BOUNDS_CHECKING);
		void* Allocate();

		void Deallocate(void* block);

		uint32 GetCountOfChunks();
		void* GetArena();
		size_t GetUsedMemory();
		size_t GetTotalMemory();

		void Clear();
		void Destroy();
		~PoolAllocator();
	private:
		void* arena; // start address
		size_t size; // size of allocated Memory
		size_t chunkSize; // size of each chunk
		size_t usedMemory;
		uint32 numChunks;

		BoundsChecking boundsChecking;

		void* freelist;

		void CreateFreeList(void* position, size_t blocksize, MemoryTracking memTracking,
			BoundsChecking boundsChecking);
		void* CreateFreeListBlock(void* position, void** newPosition, size_t blocksize, MemoryTracking memTracking,
			BoundsChecking boundsChecking);

	};
}

#endif // POOLALLOCATOR_H