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
	INVISION_USE_HEADER = 1 
};

#define INVISION_MEM_ALLOCATION_ALLIGNMENT 4

#ifndef MEMORYBLOCK_H
#define MEMORYBLOCK_H

struct SMemoryTracking
{
	int filenumber;
	void* filename;
} tempTrackingStruct;

struct SHeader
{
	unsigned int size;
	void* frontOffset;
	void* backOffset;
} tempHeader;

class HAL_API MemoryBlock
{
	public:
		void* CreateMemoryBlock(void* position,
			uint32 size,
			uint32 allocSize,
			uint32 filenumber,
			char* filename,
			UseHeader header = INVISION_USE_NO_HEADER,
			MemoryTracking memTracking = INVISION_DEFAULT_MEMORY_TRACKING,
			BoundsChecking boundsChecking = INVISION_NO_BOUNDS_CHECKING);
	private:
		uint8 ForwardAlignment(void* address, uint8 alignment);
		uint8 BackwardAlignment(void* address, uint8 alignment);

		uint8 ForwardAlignmentWithHeader(void* address, uint8 alignment, uint8 header);
		uint8 BackwardAlignmentWithHeader(void* address, uint8 alignment, uint8 header);

		void WriteToLog(std::string initMessage, void* address);
		void WriteToLog(std::string initMessage, uint32 number);

		void* Add(void* address, uint32 toAdd);
		void* Subtract(void* address, uint32 toSubtract);

		// Bound checking size
		unsigned int FRONT_SIZE = 4;
		unsigned int BACK_SIZE = 4;
};

#endif // MEMORYBLOCK_H