#include "precompiled.h"
#include "MemoryBlock.h"
#include "../../common/Log.h"



void* MemoryBlock::CreateMemoryBlock(
	void* position,
	void** endposition,
	uint32 size, 
	uint32 lineOfFile,
	char* filename, 
	UseHeader header,
	MemoryTracking memTracking, 
	BoundsChecking boundsChecking)
{
#ifdef _DEBUG
	std::stringstream ss;
	ss << std::endl << "Call Method: CreateMemoryBlock(position = 0x" << position
		<< ", ..." << ")";
	INVISION_LOG_RAWTEXT(ss.str());
#endif



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
		((SMemoryTracking*)PtrHeader)->lineOfFile = lineOfFile;
		currentPosition = (void*)PtrHeader;
#ifdef _DEBUG
		WriteToLog("    TrackingHeader: ", PtrHeader);
#endif

		currentPosition = Add(currentPosition, sizeof(SMemoryTracking));
	}

	SHeaderStack* PtrHeaderStack; // used for StackAllocator
	if (header == INVISION_USE_STACKHEADER)
	{
		// USE HEADER with size, front offset, back offset
		unsigned int adjustment = ForwardAlignment(currentPosition, INVISION_MEM_ALLOCATION_ALLIGNMENT);
		//unsigned int adjustment = ForwardAlignmentWithHeader(currentPosition, INVISION_MEM_ALLOCATION_ALLIGNMENT, sizeof(SHeader));
		PtrHeaderStack = (SHeaderStack*)Add(currentPosition, adjustment);
		*PtrHeaderStack = tempHeaderStack;
		((SHeaderStack*)PtrHeaderStack)->frontOffset = position;
		currentPosition = (void*)PtrHeaderStack;

#ifdef _DEBUG
		WriteToLog("    StackHeader: ", PtrHeaderStack);
#endif

		currentPosition = Add(currentPosition, sizeof(SHeaderStack));

	}
	else if (header == INVISION_USE_POOLHEADER)
	{
		SHeaderPool* PtrHeaderPool; // used for Pool Allocator

		// USE HEADER with pointer to next
		unsigned int adjustment = ForwardAlignment(currentPosition, INVISION_MEM_ALLOCATION_ALLIGNMENT);
		PtrHeaderPool = (SHeaderPool*)Add(currentPosition, adjustment);
		*PtrHeaderPool = tempHeaderPool;
		((SHeaderPool*)PtrHeaderPool)->next = 0x00000;
		currentPosition = (void*)PtrHeaderPool;

#ifdef _DEBUG
		WriteToLog("    PoolHeader: ", PtrHeaderPool);
#endif

		currentPosition = Add(currentPosition, sizeof(SHeaderPool));

	}

	unsigned int adjustmentSize = ForwardAlignment(currentPosition, INVISION_MEM_ALLOCATION_ALLIGNMENT);
	void* p = (void*)Add(currentPosition, adjustmentSize);
	WriteToLog("    Payload Address: ", currentPosition);

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

	if (header == INVISION_USE_STACKHEADER)
	{
		((SHeaderStack*)PtrHeaderStack)->size = memBlockSize;
		((SHeaderStack*)PtrHeaderStack)->backOffset = currentPosition;
	}
	*endposition = currentPosition;

#ifdef _DEBUG
	WriteToLog("Block Size: ", memBlockSize);
	WriteToLog("Memory Block End:", currentPosition);
#endif

	return p;
}

SHeaderStack* MemoryBlock::GetStackHeader(void* memoryBlock)
{
	void* currentPosition = memoryBlock;
	unsigned int adjustment = BackwardAlignment(currentPosition, INVISION_MEM_ALLOCATION_ALLIGNMENT);
	currentPosition  = Subtract(currentPosition, adjustment);
	unsigned int sHeaderAdjustment = BackwardAlignmentWithHeader(currentPosition, INVISION_MEM_ALLOCATION_ALLIGNMENT, sizeof(SHeaderStack));
	currentPosition = Subtract(currentPosition, sHeaderAdjustment);

#ifdef _DEBUG
	std::stringstream ss;
	ss << std::endl << "Call Method: GetStackHeader(memoryBlock = 0x" << memoryBlock << ")";
	INVISION_LOG_RAWTEXT(ss.str());
	WriteToLog("Front Offset: ", ((SHeaderStack*)currentPosition)->frontOffset);
	WriteToLog("Back Offset: ", ((SHeaderStack*)currentPosition)->backOffset);
	WriteToLog("Size: ", ((SHeaderStack*)currentPosition)->size);
#endif

	return (SHeaderStack*)currentPosition;
}

SHeaderPool* MemoryBlock::GetPoolHeader(void* memoryBlock)
{
	void* currentPosition = memoryBlock;
	unsigned int adjustment = BackwardAlignment(currentPosition, INVISION_MEM_ALLOCATION_ALLIGNMENT);
	currentPosition = Subtract(currentPosition, adjustment);
	unsigned int sHeaderAdjustment = BackwardAlignmentWithHeader(currentPosition, INVISION_MEM_ALLOCATION_ALLIGNMENT, sizeof(SHeaderPool));
	currentPosition = Subtract(currentPosition, sHeaderAdjustment);

#ifdef _DEBUG
	std::stringstream ss;
	ss << std::endl << "Call Method: GetPoolHeader(memoryBlock = 0x" << memoryBlock << ")";
	INVISION_LOG_RAWTEXT(ss.str());
	WriteToLog("Pointer to next Object: ", ((SHeaderPool*)currentPosition)->next);
#endif

	return (SHeaderPool*)currentPosition;
}

void MemoryBlock::SetPoolHeader(void* memoryBlock, size_t next)
{
	void* currentPosition = memoryBlock;
	unsigned int adjustment = BackwardAlignment(currentPosition, INVISION_MEM_ALLOCATION_ALLIGNMENT);
	currentPosition = Subtract(currentPosition, adjustment);
	unsigned int sHeaderAdjustment = BackwardAlignmentWithHeader(currentPosition, INVISION_MEM_ALLOCATION_ALLIGNMENT, sizeof(SHeaderPool));
	currentPosition = Subtract(currentPosition, sHeaderAdjustment);

	((SHeaderPool*)currentPosition)->next = (void*)next;
}

uint32 MemoryBlock::CalculateSize(void* position, uint32 size, UseHeader header,
	MemoryTracking memTracking,
	BoundsChecking boundsChecking)
{
	uint32 calcSize = 0;

	if (boundsChecking == INVISION_STANDARD_BOUNDS_CHECKING)
	{
		calcSize = ForwardAlignment(position, INVISION_MEM_ALLOCATION_ALLIGNMENT);
		calcSize += FRONT_SIZE;
	}

	if (memTracking == INVISION_ADVANCED_MEMORY_TRACKING)
	{
		calcSize += ForwardAlignment((void*)calcSize, INVISION_MEM_ALLOCATION_ALLIGNMENT);
		calcSize += sizeof(SMemoryTracking);
	}

	if (header == INVISION_USE_STACKHEADER)
	{
		calcSize += ForwardAlignment((void*)calcSize, INVISION_MEM_ALLOCATION_ALLIGNMENT);
		calcSize += sizeof(SHeaderStack);
	}
	else if (header == INVISION_USE_POOLHEADER)
	{
		calcSize += ForwardAlignment((void*)calcSize, INVISION_MEM_ALLOCATION_ALLIGNMENT);
		calcSize += sizeof(SHeaderPool);
	}

	calcSize += ForwardAlignment((void*)calcSize, INVISION_MEM_ALLOCATION_ALLIGNMENT);
	calcSize += size;

	if (boundsChecking == INVISION_STANDARD_BOUNDS_CHECKING)
	{
		calcSize += ForwardAlignment(position, INVISION_MEM_ALLOCATION_ALLIGNMENT);
		calcSize += BACK_SIZE;
	}

	return calcSize;
}

SMemoryTracking* MemoryBlock::GetTrackingHeader(void* memoryBlock, UseHeader header)
{
	void* currentPosition = memoryBlock;
	unsigned int adjustment = BackwardAlignment(currentPosition, INVISION_MEM_ALLOCATION_ALLIGNMENT);
	currentPosition = Subtract(currentPosition, adjustment);

	if (header == INVISION_USE_STACKHEADER)
	{
		unsigned int sHeaderAdjustment = BackwardAlignmentWithHeader(currentPosition, INVISION_MEM_ALLOCATION_ALLIGNMENT, sizeof(SHeaderStack));
		currentPosition = Subtract(currentPosition, sHeaderAdjustment);
	}
	else if (header == INVISION_USE_POOLHEADER)
	{
		unsigned int sHeaderAdjustment = BackwardAlignmentWithHeader(currentPosition, INVISION_MEM_ALLOCATION_ALLIGNMENT, sizeof(SHeaderPool));
		currentPosition = Subtract(currentPosition, sHeaderAdjustment);
	}

	unsigned int sTrackingAdjustment = BackwardAlignmentWithHeader(currentPosition, INVISION_MEM_ALLOCATION_ALLIGNMENT, sizeof(SMemoryTracking));
	currentPosition = Subtract(currentPosition, sTrackingAdjustment);

#ifdef _DEBUG
	std::stringstream ss;
	ss << std::endl << "Call Method: GetMemoryTracking(memoryBlock = 0x" << memoryBlock << ", header = " << header << ")";
	INVISION_LOG_RAWTEXT(ss.str());
	
	std::stringstream ssFilename;
	ssFilename << "Filename: " << (char*)((SMemoryTracking*)currentPosition)->filename;

	INVISION_LOG_RAWTEXT(ssFilename.str());
	WriteToLog("Line: ", ((SMemoryTracking*)currentPosition)->lineOfFile);
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

	if (header == INVISION_USE_STACKHEADER)
	{
		unsigned int sHeaderAdjustment = BackwardAlignmentWithHeader(pCurrentFront, INVISION_MEM_ALLOCATION_ALLIGNMENT, sizeof(SHeaderStack));
		pCurrentFront = Subtract(pCurrentFront, sHeaderAdjustment);
	}
	else if (header == INVISION_USE_POOLHEADER)
	{
		unsigned int sHeaderAdjustment = BackwardAlignmentWithHeader(pCurrentFront, INVISION_MEM_ALLOCATION_ALLIGNMENT, sizeof(SHeaderPool));
		pCurrentFront = Subtract(pCurrentFront, sHeaderAdjustment);
	}


	if (memTracking == INVISION_ADVANCED_MEMORY_TRACKING)
	{
		unsigned int sTrackingAdjustment = BackwardAlignmentWithHeader(pCurrentFront, INVISION_MEM_ALLOCATION_ALLIGNMENT, sizeof(SMemoryTracking));
		pCurrentFront = Subtract(pCurrentFront, sTrackingAdjustment);
	}

	unsigned int sFrontBoundAdjustment = BackwardAlignmentWithHeader(pCurrentFront, INVISION_MEM_ALLOCATION_ALLIGNMENT, FRONT_SIZE);
	unsigned int *FrontBoundary = (unsigned int*)Subtract(pCurrentFront, sFrontBoundAdjustment);

	// Get Front Boundary
	pCurrentBack = Add(pCurrentBack, payloudSize);
	unsigned int adjustmentSize = ForwardAlignment(pCurrentBack, INVISION_MEM_ALLOCATION_ALLIGNMENT);
	unsigned int *BackBoundary = (unsigned int*)Add(pCurrentBack, adjustmentSize);

#ifdef _DEBUG

	std::stringstream ss;
	ss << std::endl << "Call Method: CheckBoundaries(memoryBlock = 0x" << memoryBlock << ", payloadSize = " << payloudSize <<", header = " << header << ", memTracking = " << memTracking << ")";
	INVISION_LOG_RAWTEXT(ss.str());

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
#ifdef _DEBUG
	if (!isPowerOfTwo(alignment))
	{
		INVISION_LOG_WARNING("Allignment is not a power of two");
		throw invisionCoreMemoryHasWrongAlignment("Alignment has not a Power of Two!");
	}
#endif

	// faster than adjustment = alignment - (address % alignment);
	uint8 adjustment = alignment - (reinterpret_cast<size_t>(address) & static_cast<size_t>(alignment - 1));

	if (adjustment == alignment)
		return 0;

	return adjustment;
}

// apply a backward alignment to the address
uint8 MemoryBlock::BackwardAlignment(void* address, uint8 alignment)
{
#ifdef _DEBUG
	if (!isPowerOfTwo(alignment))
	{
		INVISION_LOG_WARNING("Allignment is not a power of two");
		throw invisionCoreMemoryHasWrongAlignment("Alignment has not a Power of Two!");
	}
#endif

	// faster than adjustment = address % alignment;
	uint8 adjustment = reinterpret_cast<size_t>(address) & static_cast<size_t>(alignment - 1);

	if (adjustment == alignment)
		return 0;

	return adjustment;
}

// apply a forward alignment including header to the address
uint8 MemoryBlock::ForwardAlignmentWithHeader(void* address, uint8 alignment, uint8 headersize)
{
#ifdef _DEBUG
	if (!isPowerOfTwo(alignment))
	{
		INVISION_LOG_WARNING("Allignment is not a power of two");
		throw invisionCoreMemoryHasWrongAlignment("Alignment has not a Power of Two!");
	}
#endif

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

#ifdef _DEBUG
	if (!isPowerOfTwo(alignment))
	{
		INVISION_LOG_WARNING("Allignment is not a power of two");
		throw invisionCoreMemoryHasWrongAlignment("Alignment has not a Power of Two!");
	}
#endif

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

bool MemoryBlock::isPowerOfTwo(uint32 number)
{
	return (number & (number - 1)) == 0;
}