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

#ifndef MEMORYBLOCK_H
#define MEMORYBLOCK_H

struct SMemoryTracking
{
	int lineOfFile;
	void* filename;
} tempTrackingStruct;

struct SHeaderStack
{
	unsigned int size;
	void* frontOffset;
	void* backOffset;
} tempHeaderStack;

struct SHeaderPool
{
	void* next;
}tempHeaderPool;

class HAL_API MemoryBlock
{
	public:
		void* CreateMemoryBlock(void* position,
			void** endposition,
			uint32 size,
			uint32 lineOfFile,
			char* filename,
			UseHeader header = INVISION_USE_NO_HEADER,
			MemoryTracking memTracking = INVISION_DEFAULT_MEMORY_TRACKING,
			BoundsChecking boundsChecking = INVISION_NO_BOUNDS_CHECKING);

		SHeaderStack* GetStackHeader(void* memoryBlock);
		SHeaderPool* GetPoolHeader(void* memoryBlock);
		void SetPoolHeader(void* memoryBlock, size_t next);
		uint32 CalculateSize(void* position, uint32 size, UseHeader header = INVISION_USE_NO_HEADER,
			MemoryTracking memTracking = INVISION_DEFAULT_MEMORY_TRACKING,
			BoundsChecking boundsChecking = INVISION_NO_BOUNDS_CHECKING);

		SMemoryTracking* GetTrackingHeader(void* memoryBlock, UseHeader header);

		bool CheckBoundaries(void* memoryBlock, uint32 payloudSize, UseHeader header, MemoryTracking memTracking);
	private:
		uint8 ForwardAlignment(void* address, uint8 alignment);
		uint8 BackwardAlignment(void* address, uint8 alignment);

		uint8 ForwardAlignmentWithHeader(void* address, uint8 alignment, uint8 header);
		uint8 BackwardAlignmentWithHeader(void* address, uint8 alignment, uint8 header);

		void WriteToLog(std::string initMessage, void* address);
		void WriteToLog(std::string initMessage, uint32 number);

		void* Add(void* address, uint32 toAdd);
		void* Subtract(void* address, uint32 toSubtract);

		bool isPowerOfTwo(uint32 number);

		// Bound checking size
		unsigned int FRONT_SIZE = 4;
		unsigned int BACK_SIZE = 4;
};

#endif // MEMORYBLOCK_H