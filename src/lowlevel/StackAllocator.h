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

#ifndef STACKALLOCATOR_H
#define STACKALLOCATOR_H

class HAL_API StackAllocator
{
private:
	void *arena = NULL;
	unsigned int size;
	uint8_t usedMemory;
	uint8_t numAllocations;

	void*  _current_pos;



public:

	static const uint8_t HEADERSIZE = sizeof(int);
	static const uint8_t ISDEALLOCATABLE = true;

	explicit StackAllocator(unsigned int size);
	void* allocate(uint8_t payload, uint8_t alignment, uint8_t BoundaryFrontSize, uint8_t boundaryBackSize, uint8_t MemoryTrackingSize, uint8_t line, char* file);
	uint8_t getAllocationCount();
	void* getArena();
	uint8_t getUsedMemory();
	uint8_t getTotalMemory();
	void deallocate(void* obj, uint8_t MemoryTrackingSize, uint8_t BoundaryFrontSize);
	void clear();
	~StackAllocator();

};

#endif