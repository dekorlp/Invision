#include "precompiled.h"
#include "MemoryBlock.h"
#include "../../common/Log.h"



void* MemoryBlock::CreateMemoryBlock(
	void* position, 
	uint32 size,
	uint32 allocSize, 
	uint32 filenumber,
	char* filename, 
	UseHeader header,
	MemoryTracking memTracking, 
	BoundsChecking boundsChecking)
{
	void* currentPosition = position;

#ifdef _DEBUG
	WriteToLog("Memory Block Start: ", position);
	WriteToLog("Allocation Size: ", size);
#endif

	if (boundsChecking == INVISION_STANDARD_BOUNDS_CHECKING)
	{
		// FRONT Boundary
		unsigned int *bound = (unsigned int*)currentPosition;
		*bound = 0xFAFFB;

#ifdef _DEBUG
		WriteToLog("    Front Boundary: ", currentPosition);
#endif
		currentPosition = Add(currentPosition, FRONT_SIZE);


	}

	if (memTracking == INVISION_ADVANCED_MEMORY_TRACKING)
	{
		// USE HEADER for Memory Tracking
		SMemoryTracking tempTrackingStruct;
		tempTrackingStruct.filenumber = filenumber;
		tempTrackingStruct.filename = filename;

		unsigned int adjustment = ForwardAlignmentWithHeader(currentPosition, INVISION_MEM_ALLOCATION_ALLIGNMENT, sizeof(SMemoryTracking));
		SMemoryTracking* PtrHeader = (SMemoryTracking*)Add(currentPosition, adjustment);
		*PtrHeader = tempTrackingStruct;
		currentPosition = (void*)PtrHeader;
#ifdef _DEBUG
		WriteToLog("    TrackingHeader: ", PtrHeader);
#endif

		currentPosition = Add(currentPosition, sizeof(SMemoryTracking));
	}

	SHeader* PtrHeader;
	if (header == INVISION_USE_HEADER)
	{
		// USE HEADER with size, front offset, back offset
		SHeader tempHeader;
		tempHeader.frontOffset = position;

		unsigned int adjustment = ForwardAlignmentWithHeader(currentPosition, INVISION_MEM_ALLOCATION_ALLIGNMENT, sizeof(SHeader));
		PtrHeader = (SHeader*)Add(currentPosition, adjustment);

		*PtrHeader = tempHeader;
		currentPosition = (void*)PtrHeader;

#ifdef _DEBUG
		WriteToLog("    Header(MemClass): ", PtrHeader);
#endif

		currentPosition = Add(currentPosition, sizeof(SHeader));

	}

	unsigned int adjustmentSize = ForwardAlignment(currentPosition, INVISION_MEM_ALLOCATION_ALLIGNMENT);
	void* p = (void*)Add(currentPosition, adjustmentSize);

	//int* p1 = (int*)p;
	currentPosition = (void*)Add(p, size);

	if (boundsChecking == INVISION_STANDARD_BOUNDS_CHECKING)
	{
		//BACK Boundary
		unsigned int adjustmentSize = ForwardAlignment(currentPosition, INVISION_MEM_ALLOCATION_ALLIGNMENT);
		currentPosition = Add(currentPosition, adjustmentSize);
		unsigned int *bound = (unsigned int*)currentPosition;
		*bound = 0xFAFFB;
#ifdef _DEBUG
		WriteToLog("    Back Boundary: ", currentPosition);
#endif
		currentPosition = Add(currentPosition, BACK_SIZE);
	}
	
	
	// this could be a possible assignment
	//*p1 = 255425;

	// set position of header structure
	uint32 memBlockSize = (reinterpret_cast<uint32>(currentPosition) - reinterpret_cast<uint32>(position));
	((SHeader*)PtrHeader)->size = memBlockSize;
	((SHeader*)PtrHeader)->backOffset = currentPosition;

#ifdef _DEBUG
	WriteToLog("Block Size: ", memBlockSize);
	WriteToLog("Memory Block End:", currentPosition);
#endif

	return p;
}

void MemoryBlock::WriteToLog(std::string initMessage, void* address)
{
	std::stringstream ss;
	ss << initMessage << "0x" << std::hex << address;
	INVISION_LOG_RAWTEXT(ss.str());
}

void MemoryBlock::WriteToLog(std::string initMessage, uint32 number)
{
	std::stringstream ss;
	ss << initMessage  << number;
	INVISION_LOG_RAWTEXT(ss.str());
}

// apply a forward alignment to the address
uint8 MemoryBlock::ForwardAlignment(void* address, uint8 alignment)
{
	// faster than adjustment = alignment - (address % alignment);
	uint8 adjustment = alignment - (reinterpret_cast<size_t>(address) & static_cast<size_t>(alignment - 1));

	if (adjustment == alignment)
		return 0;

	return adjustment;
}

// apply a backward alignment to the address
uint8 MemoryBlock::BackwardAlignment(void* address, uint8 alignment)
{
	// faster than adjustment = address % alignment;
	uint8 adjustment = reinterpret_cast<size_t>(address) & static_cast<size_t>(alignment - 1);

	if (adjustment == alignment)
		return 0;

	return adjustment;
}

// apply a forward alignment including header to the address
uint8 MemoryBlock::ForwardAlignmentWithHeader(void* address, uint8 alignment, uint8 headersize)
{
	uint8 adjustment = ForwardAlignment(address, alignment);

	uint8 size = headersize;

	if (adjustment < size)
	{
		size -= adjustment;
		adjustment += alignment * (size / alignment);
		if (size % alignment > 0)
		{
			adjustment += alignment;
		}
	}

	return adjustment;
}

// apply a backward alignment including header to the address
uint8 MemoryBlock::BackwardAlignmentWithHeader(void* address, uint8 alignment, uint8 headersize)
{
	uint8 adjustment = BackwardAlignment(address, alignment);

	uint8 size = headersize;

	if (adjustment < size)
	{
		size -= adjustment;
		adjustment += alignment * (size / alignment);
		if (size % alignment > 0)
		{
			adjustment += alignment;
		}
	}

	return adjustment;
}

// add an address and an value to create a new address
void* MemoryBlock::Add(void* address, uint32 toAdd)
{
	return (void*)(reinterpret_cast<size_t>(address) + toAdd);
}

// subtract a value of an address to create a new address
void* MemoryBlock::Subtract(void* address, uint32 toSubtract)
{
	return (void*)(reinterpret_cast<size_t>(address) - toSubtract);
}