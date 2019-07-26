#include "precompiled.h"
#include "MemoryBlock.h"
#include "../../common/Log.h"
#include "../PointerUtil.h"





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

	

	WriteToLog("Memory Block Start: ", position);
	WriteToLog("Allocation Site: ", size);
	if (boundsChecking == INVISION_STANDARD_BOUNDS_CHECKING)
	{
		// FRONT Boundary
		unsigned int *bound = (unsigned int*)currentPosition;
		*bound = 0xFAFFB;
		WriteToLog("    Front Boundary: ", currentPosition);
		currentPosition = Add(currentPosition, sizeof(unsigned int*));
	}

	SHeader* PtrHeader;
	if (header == INVISION_USE_HEADER)
	{
		// USE HEADER with size, front offset, back offset
		SHeader tempHeader;
		tempHeader.frontOffset = position;

		unsigned int adjustment = ForwardAlignmentWithHeader(currentPosition, 4, sizeof(SHeader));
		PtrHeader = (SHeader*) Add(currentPosition, adjustment);

		*PtrHeader = tempHeader;
		currentPosition = (void*)PtrHeader;
		WriteToLog("    Header(MemClass): ", PtrHeader);
		currentPosition = Add(currentPosition, sizeof(SHeader));

	}

	if (memTracking == INVISION_ADVANCED_MEMORY_TRACKING)
	{
		// USE HEADER for Memory Tracking
		SMemoryTracking tempTrackingStruct;
		tempTrackingStruct.filenumber = filenumber;
		tempTrackingStruct.filename = filename;

		unsigned int adjustment = ForwardAlignmentWithHeader(currentPosition, 4, sizeof(SMemoryTracking));
		SMemoryTracking* PtrHeader = (SMemoryTracking*)Add(currentPosition, adjustment);
		*PtrHeader = tempTrackingStruct;
		currentPosition = (void*)PtrHeader;
		WriteToLog("    TrackingHeader: ", PtrHeader);
		currentPosition = Add(currentPosition, sizeof(SMemoryTracking));
	}

	unsigned int adjustmentSize = ForwardAlignment(currentPosition, 4);
	void* p = (void*)Add(currentPosition, adjustmentSize);

	//int* p1 = (int*)p;
	currentPosition = (void*)Add(p, size);

	if (boundsChecking == INVISION_STANDARD_BOUNDS_CHECKING)
	{
		//BACK Boundary
		unsigned int adjustmentSize = ForwardAlignment(currentPosition, 4);
		currentPosition = Add(currentPosition, adjustmentSize);
		unsigned int *bound = (unsigned int*)currentPosition;
		*bound = 0xFAFFB;
		WriteToLog("    Back Boundary: ", currentPosition);
		currentPosition = Add(currentPosition, sizeof(unsigned int*));
	}
	WriteToLog("Memory Block End:", currentPosition);
	
	// this could be a possible assignment
	//*p1 = 255425;

	// set position of header structure
	

	((SHeader*)PtrHeader)->size = reinterpret_cast<int32>(currentPosition) - reinterpret_cast<int32>(position);
	((SHeader*)PtrHeader)->backOffset = currentPosition;
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
	uint8 adjustment = alignment - (reinterpret_cast<unsigned int>(address) & static_cast<unsigned int>(alignment - 1));

	if (adjustment == alignment)
		return 0;

	return adjustment;
}

// apply a backward alignment to the address
uint8 MemoryBlock::BackwardAlignment(void* address, uint8 alignment)
{
	// faster than adjustment = address % alignment;
	uint8 adjustment = reinterpret_cast<unsigned int>(address) & static_cast<unsigned int>(alignment - 1);

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
	return (void*)(reinterpret_cast<unsigned int>(address) + toAdd);
}

// subtract a value of an address to create a new address
void* MemoryBlock::Subtract(void* address, uint32 toSubtract)
{
	return (void*)(reinterpret_cast<unsigned int>(address) - toSubtract);
}