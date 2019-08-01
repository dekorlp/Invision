/////////////////////////////////////////////////////////////////////////////
// Name:	  LinearAllocator
// Purpose:	  allocator subComponent allocator who allows Linear Allocation
// Author:	  Dennis Koehler
// Created:	  2015-06-12
// Copyright: Copyright (c) 2015, Nano Game Foundation
//			  This unpublished source code is proprietary to Nano Game Foundation.
//			  The source code herein is considered trade secrets and confidential.
//			  Reproduction or distribution, in part or complete, is prohibited except by permission of Nano Game Foundation. 
//			  All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#ifndef LINEARALLOCATOR_H
#define LINEARALLOCATOR_H


class HAL_API LinearAllocator
{
	public:
		void init(size_t size);
		void* Allocate(size_t blocksize, uint32 line, char* file, MemoryTracking memTracking,
			BoundsChecking boundsChecking);

		uint32 getCountOfChunks();
		void* getArena();
		size_t getUsedMemory();
		size_t getTotalMemory();

		void clear();
	private:
		void* arena; // start address
		void* currentOffset; // boundary between used and free memory
		size_t size; // size of allocated Memory
		size_t usedMemory;
		uint32 numChunks; 

};


#endif //LINEARALLOCATOR_H