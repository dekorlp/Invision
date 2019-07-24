/////////////////////////////////////////////////////////////////////////////
// Name:	  LinearAllocator
// Purpose:	  allocator subComponent allocator who allows Linear Allocation
// Author:	  Dennis Koehler
// Created:	  2016-09-24
// Copyright: Copyright (c) 2016, Nano Game Foundation
//			  This unpublished source code is proprietary to Nano Game Foundation.
//			  The source code herein is considered trade secrets and confidential.
//			  Reproduction or distribution, in part or complete, is prohibited except by permission of Nano Game Foundation. 
//			  All rights reserved.
/////////////////////////////////////////////////////////////////////////////



#ifndef FREELISTALLOCATOR_H
#define FREELISTALLOCATOR_H

class HAL_API FreeListAllocator
{
private:

	struct FreeBlock
	{
		size_t     size;
		FreeBlock* next;
	};

	void *arena = NULL;
	size_t size;
	size_t usedMemory;
	size_t numAllocations;
	void*  _current_pos;
	FreeBlock* free_blocks;

public:

	static const size_t HEADERSIZE = sizeof(int);
	static const size_t ISDEALLOCATABLE = true;

	explicit FreeListAllocator(size_t size);
	void* allocate(size_t blocksize, size_t alignment, size_t offset, size_t boundaryBackSize, size_t line, char* file);
	size_t getAllocationCount();
	void* getArena();
	size_t getUsedMemory();
	size_t getTotalMemory();
	void deallocate(void* obj, size_t blocksize, size_t alignment, size_t offset);
	void clear();
	~FreeListAllocator();

};

#endif