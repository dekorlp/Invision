#include "precompiled.h"
#include "StackAllocator.h"
#include "PointerUtil.h"

StackAllocator::StackAllocator(unsigned int size)
{
	arena = operator new (size);
	this->size = size;
	usedMemory = 0;
	numAllocations = 0;
	_current_pos = arena;
}

void* StackAllocator::allocate(uint8_t payload, uint8_t alignment, uint8_t BoundaryFrontSize, uint8_t boundaryBackSize, uint8_t MemoryTrackingSize, uint8_t line, char* file)
{

	int headersize = this->HEADERSIZE;

	uint8_t adjustmentForward = PointerUtil::alignForwardAdjustmentWithHeader(_current_pos, alignment, headersize + BoundaryFrontSize + MemoryTrackingSize);
	uint8_t adjustmentBackward = PointerUtil::alignBackwardAdjustmentWithHeader(_current_pos, alignment, boundaryBackSize);
	uint8_t blocksize = adjustmentForward + payload + adjustmentBackward;
	
#ifdef _DEBUG
	if (usedMemory >= size)
	{
		throw invisionCoreOutOfMemory("There is not enough additional memory available!");
	}
#endif

	void *p = PointerUtil::add(_current_pos, adjustmentForward);
	
	if (headersize > 0)
	{
		uint8_t *header = (uint8_t*)(PointerUtil::subtract(p, MemoryTrackingSize + headersize));
		*header = blocksize;
	}

	_current_pos = (void*)(PointerUtil::add(p, MemoryTrackingSize));

	usedMemory += blocksize;
	numAllocations += 1;

	return p; (PointerUtil::add(p, headersize));
}

uint8_t StackAllocator::getAllocationCount()
{
	return numAllocations;
}

void* StackAllocator::getArena()
{
	return arena;
}

uint8_t StackAllocator::getUsedMemory()
{
	return usedMemory;
}

uint8_t StackAllocator::getTotalMemory()
{
	return size;
}

void StackAllocator::deallocate(void* obj, uint8_t MemoryTrackingSize, uint8_t BoundaryFrontSize)
{
	_current_pos = PointerUtil::subtract(obj, MemoryTrackingSize);
	_current_pos = PointerUtil::subtract(_current_pos, HEADERSIZE);
	 uint8_t *blocksize = (uint8_t*)_current_pos;
	_current_pos = PointerUtil::subtract(_current_pos, BoundaryFrontSize);


	numAllocations -= 1;
	usedMemory -= *blocksize;

	
}

void StackAllocator::clear()
{
	numAllocations = 0;
	usedMemory = 0;
	_current_pos = arena;
}

StackAllocator::~StackAllocator()
{
	_current_pos = nullptr;
	delete arena;
}