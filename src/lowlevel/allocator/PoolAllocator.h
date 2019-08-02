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


class HAL_API PoolAllocator
{
public:
	void Init(size_t size, size_t chunksize);
	void* Allocate(size_t blocksize, uint32 line, char* file, MemoryTracking memTracking,
		BoundsChecking boundsChecking);

	void Deallocate(void* block);

	uint32 GetCountOfChunks();
	void* GetArena();
	size_t GetUsedMemory();
	size_t GetTotalMemory();

	void Clear();
private:
	void* arena; // start address
	//void* currentOffset; // boundary between used and free memory
	size_t size; // size of allocated Memory
	size_t chunkSize; // size of each chunk
	size_t usedMemory;
	uint32 numChunks;
	void* freelist;

	void CreateFreeList(void* position, size_t blocksize, uint32 line, char* file, MemoryTracking memTracking,
		BoundsChecking boundsChecking);
	void* CreateFreeListBlock(void* position, void** newPosition, size_t blocksize, uint32 line, char* file, MemoryTracking memTracking,
		BoundsChecking boundsChecking);

};


#endif // POOLALLOCATOR_H