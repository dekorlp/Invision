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
		unsigned int adjustment = ForwardAlignment(currentPosition, INVISION_MEM_ALLOCATION_ALLIGNMENT);
		currentPosition = Add(currentPosition, adjustment);

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

		unsigned int adjustment = ForwardAlignment(currentPosition, INVISION_MEM_ALLOCATION_ALLIGNMENT);
		//unsigned int adjustment = ForwardAlignmentWithHeader(currentPosition, INVISION_MEM_ALLOCATION_ALLIGNMENT, sizeof(SMemoryTracking));
		SMemoryTracking* PtrHeader = (SMemoryTracking*)Add(currentPosition, adjustment);
		*PtrHeader = tempTrackingStruct;
		((SMemoryTracking*)PtrHeader)->filename = filename;
		((SMemoryTracking*)PtrHeader)->filenumber = filenumber;
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
		unsigned int adjustment = ForwardAlignment(currentPosition, INVISION_MEM_ALLOCATION_ALLIGNMENT);
		//unsigned int adjustment = ForwardAlignmentWithHeader(currentPosition, INVISION_MEM_ALLOCATION_ALLIGNMENT, sizeof(SHeader));
		PtrHeader = (SHeader*)Add(currentPosition, adjustment);
		*PtrHeader = tempHeader;
		((SHeader*)PtrHeader)->frontOffset = position;
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

SHeader* MemoryBlock::GetHeader(void* memoryBlock)
{
	void* currentPosition = memoryBlock;
	unsigned int adjustment = BackwardAlignment(currentPosition, INVISION_MEM_ALLOCATION_ALLIGNMENT);
	currentPosition  = Subtract(currentPosition, adjustment);
	unsigned int sHeaderAdjustment = BackwardAlignmentWithHeader(currentPosition, INVISION_MEM_ALLOCATION_ALLIGNMENT, sizeof(SHeader));
	currentPosition = Subtract(currentPosition, sHeaderAdjustment);

#ifdef _DEBUG
	std::stringstream ss;
	ss << std::endl << "Call Method: GetHeader( memoryBlock = 0x" << memoryBlock << ")";
	INVISION_LOG_RAWTEXT(ss.str());
	WriteToLog("Front Offset: ", ((SHeader*)currentPosition)->frontOffset);
	WriteToLog("Back Offset: ", ((SHeader*)currentPosition)->backOffset);
	WriteToLog("Size: ", ((SHeader*)currentPosition)->size);
#endif

	return (SHeader*)currentPosition;
}

SMemoryTracking* MemoryBlock::GetTrackingHeader(void* memoryBlock, UseHeader header)
{
	void* currentPosition = memoryBlock;
	unsigned int adjustment = BackwardAlignment(currentPosition, INVISION_MEM_ALLOCATION_ALLIGNMENT);
	currentPosition = Subtract(currentPosition, adjustment);

	if (header == INVISION_USE_HEADER)
	{
		unsigned int sHeaderAdjustment = BackwardAlignmentWithHeader(currentPosition, INVISION_MEM_ALLOCATION_ALLIGNMENT, sizeof(SHeader));
		currentPosition = Subtract(currentPosition, sHeaderAdjustment);
	}
	

	unsigned int sTrackingAdjustment = BackwardAlignmentWithHeader(currentPosition, INVISION_MEM_ALLOCATION_ALLIGNMENT, sizeof(SMemoryTracking));
	currentPosition = Subtract(currentPosition, sTrackingAdjustment);

#ifdef _DEBUG
	std::stringstream ss;
	ss << std::endl << "Call Method: GetMemoryTracking( memoryBlock = 0x" << memoryBlock << ", header = " << header << ")";
	INVISION_LOG_RAWTEXT(ss.str());
	
	std::stringstream ssFilename;
	ssFilename << "Filename: " << (char*)((SMemoryTracking*)currentPosition)->filename;

	INVISION_LOG_RAWTEXT(ssFilename.str());
	WriteToLog("Filenumber: ", ((SMemoryTracking*)currentPosition)->filenumber);
#endif

	return (SMemoryTracking*)currentPosition;
}

bool MemoryBlock::CheckBoundaries(void* memoryBlock,  uint32 payloudSize, UseHeader header, MemoryTracking memTracking)
{
	void* pCurrentFront = memoryBlock;
	void* pCurrentBack = memoryBlock;

	// Get Back Boundary
	unsigned int adjustment = BackwardAlignment(pCurrentFront, INVISION_MEM_ALLOCATION_ALLIGNMENT);
	pCurrentFront = Subtract(pCurrentFront, adjustment);

	if (header == INVISION_USE_HEADER)
	{
		unsigned int sHeaderAdjustment = BackwardAlignmentWithHeader(pCurrentFront, INVISION_MEM_ALLOCATION_ALLIGNMENT, sizeof(SHeader));
		pCurrentFront = Subtract(pCurrentFront, sHeaderAdjustment);
	}

	if (memTracking == INVISION_ADVANCED_MEMORY_TRACKING)
	{
		unsigned int sTrackingAdjustment = BackwardAlignmentWithHeader(pCurrentFront, INVISION_MEM_ALLOCATION_ALLIGNMENT, sizeof(SMemoryTracking));
		pCurrentFront = Subtract(pCurrentFront, sTrackingAdjustment);
	}

	unsigned int * FrontBoundary = (unsigned int*)Subtract(pCurrentFront, FRONT_SIZE);

	// Get Front Boundary
	pCurrentBack = Add(pCurrentBack, payloudSize);
	unsigned int adjustmentSize = ForwardAlignment(pCurrentBack, INVISION_MEM_ALLOCATION_ALLIGNMENT);
	unsigned int *BackBoundary = (unsigned int*)Add(pCurrentBack, adjustmentSize);

#ifdef _DEBUG
	INVISION_LOG_RAWTEXT("");
	if (*FrontBoundary == 0xFAFFB)
	{
		INVISION_LOG_RAWTEXT("Front Boundary: DETECTED");
	}
	else
	{
		//INVISION_LOG_RAWTEXT("Front Boundary: NOT DETECTED");
		INVISION_LOG_ERROR("Front Boundary : NOT DETECTED");
	}

	if (*BackBoundary == 0xFAFFB)
	{
		INVISION_LOG_RAWTEXT("Back Boundary: DETECTED");
	}
	else
	{
		INVISION_LOG_ERROR("Back Boundary : NOT DETECTED");
	}
#endif

	if (*FrontBoundary == 0xFAFFB && *BackBoundary == 0xFAFFB)
	{
		return true;
	}

#ifdef _DEBUG
	INVISION_LOG_ERROR("EXCEPTION OCCURED: Memory is corrupted!");
	throw invisionCoreOutOfMemory("Memory is corrupted!");
#endif 

	return false;
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