#include "precompiled.h"
#include "MemoryBlock.h"
#include "Log.h"


namespace Invision {
	void* MemoryBlock::CreateMemoryBlock(
		void* position,
		void** endposition,
		size_t size,
		uint32 lineOfFile,
		char* filename,
		UseHeader header,
		MemoryTracking memTracking,
		BoundsChecking boundsChecking)
	{
		SMemoryTracking tempTrackingStruct = {};
		SHeaderStack tempHeaderStack = {};
		SHeaderPool tempHeaderPool = {};


//#ifdef _DEBUG
//		std::stringstream ss;
//		ss << std::endl << "Call Method: CreateMemoryBlock(position = 0x" << position
//			<< ", ..." << ")";
//		INVISION_LOG_RAWTEXT(ss.str());
//#endif



		void* currentPosition = position;

//#ifdef _DEBUG
//		Log::GetLogger()->WriteToLog("Memory Block Start: ", position);
//		Log::GetLogger()->WriteToLog("Allocation Size: ", size);
//#endif

		if (boundsChecking == INVISION_STANDARD_BOUNDS_CHECKING)
		{
			// FRONT Boundary
			unsigned int adjustment = ForwardAlignment(currentPosition, INVISION_MEM_ALLOCATION_ALLIGNMENT);
			currentPosition = Add(currentPosition, adjustment);

			unsigned int *bound = (unsigned int*)currentPosition;
			*bound = 0xFAFFB;

//#ifdef _DEBUG
//			Log::GetLogger()->WriteToLog("    Front Boundary: ", currentPosition);
//#endif
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
//#ifdef _DEBUG
//			Log::GetLogger()->WriteToLog("    TrackingHeader: ", PtrHeader);
//#endif

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

//#ifdef _DEBUG
//			Log::GetLogger()->WriteToLog("    StackHeader: ", PtrHeaderStack);
//#endif

			currentPosition = Add(currentPosition, sizeof(SHeaderStack));

		}
		else if (header == INVISION_USE_POOLHEADER)
		{
			SHeaderPool* PtrHeaderPool; // used for Pool Allocator

			// USE HEADER with pointer to next
			unsigned int adjustment = ForwardAlignment(currentPosition, INVISION_MEM_ALLOCATION_ALLIGNMENT);
			PtrHeaderPool = (SHeaderPool*)Add(currentPosition, adjustment);
			*PtrHeaderPool = tempHeaderPool;
			((SHeaderPool*)PtrHeaderPool)->next = nullptr;
			currentPosition = (void*)PtrHeaderPool;

//#ifdef _DEBUG
//			Log::GetLogger()->WriteToLog("    PoolHeader: ", PtrHeaderPool);
//#endif

			currentPosition = Add(currentPosition, sizeof(SHeaderPool));

		}

		unsigned int adjustmentSize = ForwardAlignment(currentPosition, INVISION_MEM_ALLOCATION_ALLIGNMENT);
		void* p = (void*)Add(currentPosition, adjustmentSize);
//#ifdef _DEBUG
//		Log::GetLogger()->WriteToLog("    Payload Address: ", currentPosition);
//#endif

		//int* p1 = (int*)p;
		currentPosition = (void*)Add(p, size);

		if (boundsChecking == INVISION_STANDARD_BOUNDS_CHECKING)
		{
			//BACK Boundary
			unsigned int adjustmentSize = ForwardAlignment(currentPosition, INVISION_MEM_ALLOCATION_ALLIGNMENT);
			currentPosition = Add(currentPosition, adjustmentSize);
			unsigned int *bound = (unsigned int*)currentPosition;
			*bound = 0xFAFFB;
//#ifdef _DEBUG
//			Log::GetLogger()->WriteToLog("    Back Boundary: ", currentPosition);
//#endif
			currentPosition = Add(currentPosition, BACK_SIZE);
		}


		// this could be a possible assignment
		//*p1 = 255425;

		// set position of header structure
		size_t memBlockSize = (reinterpret_cast<size_t>(currentPosition) - reinterpret_cast<size_t>(position));

		if (header == INVISION_USE_STACKHEADER)
		{
			((SHeaderStack*)PtrHeaderStack)->size = memBlockSize;
			((SHeaderStack*)PtrHeaderStack)->backOffset = currentPosition;
		}
		*endposition = currentPosition;

//#ifdef _DEBUG
//		Log::GetLogger()->WriteToLog("Block Size: ", memBlockSize);
//		Log::GetLogger()->WriteToLog("Memory Block End:", currentPosition);
//#endif

		return p;
	}

	SHeaderStack* MemoryBlock::GetStackHeader(void* memoryBlock)
	{
		void* currentPosition = memoryBlock;
		unsigned int adjustment = BackwardAlignment(currentPosition, INVISION_MEM_ALLOCATION_ALLIGNMENT);
		currentPosition = Subtract(currentPosition, adjustment);
		unsigned int sHeaderAdjustment = BackwardAlignmentWithHeader(currentPosition, INVISION_MEM_ALLOCATION_ALLIGNMENT, sizeof(SHeaderStack));
		currentPosition = Subtract(currentPosition, sHeaderAdjustment);

//#ifdef _DEBUG
//		std::stringstream ss;
//		ss << std::endl << "Call Method: GetStackHeader(memoryBlock = 0x" << memoryBlock << ")";
//		INVISION_LOG_RAWTEXT(ss.str());
//		Log::GetLogger()->WriteToLog("Front Offset: ", ((SHeaderStack*)currentPosition)->frontOffset);
//		Log::GetLogger()->WriteToLog("Back Offset: ", ((SHeaderStack*)currentPosition)->backOffset);
//		Log::GetLogger()->WriteToLog("Size: ", ((SHeaderStack*)currentPosition)->size);
//#endif

		return (SHeaderStack*)currentPosition;
	}

	SHeaderPool* MemoryBlock::GetPoolHeader(void* memoryBlock)
	{
		void* currentPosition = memoryBlock;
		unsigned int adjustment = BackwardAlignment(currentPosition, INVISION_MEM_ALLOCATION_ALLIGNMENT);
		currentPosition = Subtract(currentPosition, adjustment);
		unsigned int sHeaderAdjustment = BackwardAlignmentWithHeader(currentPosition, INVISION_MEM_ALLOCATION_ALLIGNMENT, sizeof(SHeaderPool));
		currentPosition = Subtract(currentPosition, sHeaderAdjustment);

//#ifdef _DEBUG
//		std::stringstream ss;
//		ss << std::endl << "Call Method: GetPoolHeader(memoryBlock = 0x" << memoryBlock << ")";
//		INVISION_LOG_RAWTEXT(ss.str());
//		Log::GetLogger()->WriteToLog("Pointer to next Object: ", ((SHeaderPool*)currentPosition)->next);
//#endif

		return (SHeaderPool*)currentPosition;
	}

	void MemoryBlock::SetPoolHeader(void* memoryBlock, void* next)
	{
		void* currentPosition = memoryBlock;
		unsigned int adjustment = BackwardAlignment(currentPosition, INVISION_MEM_ALLOCATION_ALLIGNMENT);
		currentPosition = Subtract(currentPosition, adjustment);
		unsigned int sHeaderAdjustment = BackwardAlignmentWithHeader(currentPosition, INVISION_MEM_ALLOCATION_ALLIGNMENT, sizeof(SHeaderPool));
		currentPosition = Subtract(currentPosition, sHeaderAdjustment);

		((SHeaderPool*)currentPosition)->next = next;
	}

	size_t MemoryBlock::CalculateSize(void* position, size_t size, UseHeader header,
		MemoryTracking memTracking,
		BoundsChecking boundsChecking)
	{
		void* currentPosition = position;


		if (boundsChecking == INVISION_STANDARD_BOUNDS_CHECKING)
		{
			// FRONT Boundary
			unsigned int adjustment = ForwardAlignment(currentPosition, INVISION_MEM_ALLOCATION_ALLIGNMENT);
			currentPosition = Add(currentPosition, adjustment + FRONT_SIZE);
		}

		if (memTracking == INVISION_ADVANCED_MEMORY_TRACKING)
		{
			// USE HEADER for Memory Tracking

			unsigned int adjustment = ForwardAlignment(currentPosition, INVISION_MEM_ALLOCATION_ALLIGNMENT);
			currentPosition = Add(currentPosition, adjustment + sizeof(SMemoryTracking));
		}

		if (header == INVISION_USE_STACKHEADER)
		{
			// USE HEADER with size, front offset, back offset
			unsigned int adjustment = ForwardAlignment(currentPosition, INVISION_MEM_ALLOCATION_ALLIGNMENT);
			currentPosition = Add(currentPosition, adjustment + sizeof(SHeaderStack));

		}
		else if (header == INVISION_USE_POOLHEADER)
		{
			// USE HEADER with size, front offset, back offset
			unsigned int adjustment = ForwardAlignment(currentPosition, INVISION_MEM_ALLOCATION_ALLIGNMENT);
			currentPosition = Add(currentPosition, adjustment + sizeof(SHeaderPool));
		}

		unsigned int adjustmentSize = ForwardAlignment(currentPosition, INVISION_MEM_ALLOCATION_ALLIGNMENT);
		currentPosition = (void*)Add(currentPosition, adjustmentSize + size);

		if (boundsChecking == INVISION_STANDARD_BOUNDS_CHECKING)
		{
			//BACK Boundary
			unsigned int adjustmentSize = ForwardAlignment(currentPosition, INVISION_MEM_ALLOCATION_ALLIGNMENT);
			currentPosition = Add(currentPosition, adjustmentSize + BACK_SIZE);
		}

		// set position of header structure
		size_t memBlockSize = (reinterpret_cast<size_t>(currentPosition) - reinterpret_cast<size_t>(position));
		return memBlockSize;
	}

	size_t MemoryBlock::CalculateLayoutSize(UseHeader header,
		MemoryTracking memTracking,
		BoundsChecking boundsChecking)
	{
		size_t layoutSize = 0;

		if (boundsChecking == INVISION_STANDARD_BOUNDS_CHECKING)
		{
			// FRONT Boundary
			layoutSize += FRONT_SIZE;
		}

		if (memTracking == INVISION_ADVANCED_MEMORY_TRACKING)
		{
			// USE HEADER for Memory Tracking
			layoutSize += sizeof(SMemoryTracking);
		}

		if (header == INVISION_USE_STACKHEADER)
		{
			// USE HEADER with size, front offset, back offset
			layoutSize += sizeof(SHeaderStack);

		}
		else if (header == INVISION_USE_POOLHEADER)
		{
			// USE HEADER with size, front offset, back offset
			layoutSize += sizeof(SHeaderPool);
		}

		if (boundsChecking == INVISION_STANDARD_BOUNDS_CHECKING)
		{
			//BACK Boundary
			layoutSize += BACK_SIZE;
		}

		// size of layout structures
		return layoutSize;
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

//#ifdef _DEBUG
//		std::stringstream ss;
//		ss << std::endl << "Call Method: GetMemoryTracking(memoryBlock = 0x" << memoryBlock << ", header = " << header << ")";
//		INVISION_LOG_RAWTEXT(ss.str());
//
//		std::stringstream ssFilename;
//		ssFilename << "Filename: " << (char*)((SMemoryTracking*)currentPosition)->filename;
//
//		INVISION_LOG_RAWTEXT(ssFilename.str());
//		Log::GetLogger()->WriteToLog("Line: ", ((SMemoryTracking*)currentPosition)->lineOfFile);
//#endif

		return (SMemoryTracking*)currentPosition;
	}

	bool MemoryBlock::CheckBoundaries(void* memoryBlock, size_t payloudSize, UseHeader header, MemoryTracking memTracking)
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

//#ifdef _DEBUG
//
//		std::stringstream ss;
//		ss << std::endl << "Call Method: CheckBoundaries(memoryBlock = 0x" << memoryBlock << ", payloadSize = " << payloudSize << ", header = " << header << ", memTracking = " << memTracking << ")";
//		INVISION_LOG_RAWTEXT(ss.str());
//
//		if (*FrontBoundary == 0xFAFFB)
//		{
//			INVISION_LOG_RAWTEXT("Front Boundary: DETECTED");
//		}
//		else
//		{
//			//INVISION_LOG_RAWTEXT("Front Boundary: NOT DETECTED");
//			INVISION_LOG_ERROR("Front Boundary : NOT DETECTED");
//		}
//
//		if (*BackBoundary == 0xFAFFB)
//		{
//			INVISION_LOG_RAWTEXT("Back Boundary: DETECTED");
//		}
//		else
//		{
//			INVISION_LOG_ERROR("Back Boundary : NOT DETECTED");
//		}
//#endif

		if (*FrontBoundary == 0xFAFFB && *BackBoundary == 0xFAFFB)
		{
			return true;
		}

//#ifdef _DEBUG
//		INVISION_LOG_ERROR("EXCEPTION OCCURED: Memory is corrupted!");
//		throw InvisionOutOfMemory("Memory is corrupted!");
//#endif 

		return false;
	}

	// apply a forward alignment to the address
	uint8 MemoryBlock::ForwardAlignment(void* address, uint8 alignment)
	{
//#ifdef _DEBUG
//		if (!isPowerOfTwo(alignment))
//		{
//			INVISION_LOG_WARNING("Allignment is not a power of two");
//			throw InvisionMemoryHasWrongAlignment("Alignment has not a Power of Two!");
//		}
//#endif

		// faster than adjustment = alignment - (address % alignment);
		uint8 adjustment = alignment - (reinterpret_cast<size_t>(address) & static_cast<size_t>(alignment - 1));

		if (adjustment == alignment)
			return 0;

		return adjustment;
	}

	// apply a backward alignment to the address
	uint8 MemoryBlock::BackwardAlignment(void* address, uint8 alignment)
	{
//#ifdef _DEBUG
//		if (!isPowerOfTwo(alignment))
//		{
//			INVISION_LOG_WARNING("Allignment is not a power of two");
//			throw InvisionMemoryHasWrongAlignment("Alignment has not a Power of Two!");
//		}
//#endif

		// faster than adjustment = address % alignment;
		uint8 adjustment = reinterpret_cast<size_t>(address) & static_cast<size_t>(alignment - 1);

		if (adjustment == alignment)
			return 0;

		return adjustment;
	}

	// apply a forward alignment including header to the address
	uint8 MemoryBlock::ForwardAlignmentWithHeader(void* address, uint8 alignment, uint8 headersize)
	{
//#ifdef _DEBUG
//		if (!isPowerOfTwo(alignment))
//		{
//			INVISION_LOG_WARNING("Allignment is not a power of two");
//			throw InvisionMemoryHasWrongAlignment("Alignment has not a Power of Two!");
//		}
//#endif

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

//#ifdef _DEBUG
//		if (!isPowerOfTwo(alignment))
//		{
//			INVISION_LOG_WARNING("Allignment is not a power of two");
//			throw InvisionMemoryHasWrongAlignment("Alignment has not a Power of Two!");
//		}
//#endif

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
	void* MemoryBlock::Add(void* address, size_t toAdd)
	{
		return (void*)(reinterpret_cast<size_t>(address) + toAdd);
	}

	// subtract a value of an address to create a new address
	void* MemoryBlock::Subtract(void* address, size_t toSubtract)
	{
		return (void*)(reinterpret_cast<size_t>(address) - toSubtract);
	}

	bool MemoryBlock::isPowerOfTwo(size_t number)
	{
		return (number & (number - 1)) == 0;
	}

}