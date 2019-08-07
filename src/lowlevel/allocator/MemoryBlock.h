/////////////////////////////////////////////////////////////////////////////
// Name:	  MemoryBlock
// Purpose:	  MemoryBlock Components, who creates a full Memory Block
// Author:	  Dennis Koehler
// Created:	  2015-06-12
// Copyright: Copyright (c) 2015, Nano Game Foundation
//			  This unpublished source code is proprietary to Nano Game Foundation.
//			  The source code herein is considered trade secrets and confidential.
//			  Reproduction or distribution, in part or complete, is prohibited except by permission of Nano Game Foundation. 
//			  All rights reserved.
/////////////////////////////////////////////////////////////////////////////




#ifndef MEMORYBLOCK_H
#define MEMORYBLOCK_H

enum MemoryTracking
{
	INVISION_DEFAULT_MEMORY_TRACKING = 0,
	INVISION_ADVANCED_MEMORY_TRACKING = 1
};

enum BoundsChecking
{
	INVISION_STANDARD_BOUNDS_CHECKING = 0,
	INVISION_NO_BOUNDS_CHECKING = 1
};

enum UseHeader
{
	INVISION_USE_NO_HEADER = 0,
	INVISION_USE_STACKHEADER = 1,
	INVISION_USE_POOLHEADER = 2
};

#define INVISION_MEM_ALLOCATION_ALLIGNMENT sizeof(char*)

struct SMemoryTracking
{
	int lineOfFile;
	void* filename;
};

struct SHeaderStack
{
	size_t size;
	void* frontOffset;
	void* backOffset;
};

struct SHeaderPool
{
	void* next;
};

class INVISION_API MemoryBlock
{
	public:
		static void* CreateMemoryBlock(void* position,
			void** endposition,
			size_t size,
			uint32 lineOfFile,
			char* filename,
			UseHeader header = INVISION_USE_NO_HEADER,
			MemoryTracking memTracking = INVISION_DEFAULT_MEMORY_TRACKING,
			BoundsChecking boundsChecking = INVISION_NO_BOUNDS_CHECKING);

		static SHeaderStack* GetStackHeader(void* memoryBlock);
		static SHeaderPool* GetPoolHeader(void* memoryBlock);
		static void SetPoolHeader(void* memoryBlock, void* next);
		static size_t CalculateSize(void* position, size_t size, UseHeader header = INVISION_USE_NO_HEADER,
			MemoryTracking memTracking = INVISION_DEFAULT_MEMORY_TRACKING,
			BoundsChecking boundsChecking = INVISION_NO_BOUNDS_CHECKING);

		static SMemoryTracking* GetTrackingHeader(void* memoryBlock, UseHeader header);

		static bool CheckBoundaries(void* memoryBlock, size_t payloudSize, UseHeader header, MemoryTracking memTracking);
	private:
		

		static uint8 ForwardAlignment(void* address, uint8 alignment);
		static uint8 BackwardAlignment(void* address, uint8 alignment);

		static uint8 ForwardAlignmentWithHeader(void* address, uint8 alignment, uint8 header);
		static uint8 BackwardAlignmentWithHeader(void* address, uint8 alignment, uint8 header);

		

		static void* Add(void* address, size_t toAdd);
		static void* Subtract(void* address, size_t toSubtract);

		static bool isPowerOfTwo(size_t number);

		// Bound checking size
		static const unsigned int FRONT_SIZE = 4;
		static const unsigned int BACK_SIZE = 4;
};

#endif // MEMORYBLOCK_H