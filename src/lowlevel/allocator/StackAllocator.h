/////////////////////////////////////////////////////////////////////////////
// Name:	  StackAllocator
// Purpose:	  allocator subComponent allocator who allows Stack based Allocation
// Author:	  Dennis Koehler
// Created:	  2016-09-24
// Copyright: Copyright (c) 2016, Nano Game Foundation
//			  This unpublished source code is proprietary to Nano Game Foundation.
//			  The source code herein is considered trade secrets and confidential.
//			  Reproduction or distribution, in part or complete, is prohibited except by permission of Nano Game Foundation. 
//			  All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#ifndef STACKALLOCATOR_H
#define STACKALLOCATOR_H


class HAL_API StackAllocator
{
public:
	void Init(size_t size);
	void* Allocate(size_t blocksize, uint32 line = 0, char* file = "", MemoryTracking memTracking = INVISION_DEFAULT_MEMORY_TRACKING,
		BoundsChecking boundsChecking = INVISION_NO_BOUNDS_CHECKING);
	void Deallocate(void* block);

	uint32 GetCountOfChunks();
	void* GetArena();
	size_t GetUsedMemory();
	size_t GetTotalMemory();

	void Clear();
private:
	void* arena; // start address
	void* currentOffset; // boundary between used and free memory
	size_t size; // size of allocated Memory
	size_t usedMemory;
	uint32 numChunks;

};


#endif //STACKALLOCATOR_H