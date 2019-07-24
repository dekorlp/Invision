#include "precompiled.h"

#include "PointerUtil.h"
#include "LinearAllocator.h"

LinearAllocator::LinearAllocator(uint8_t size)
{
	arena = operator new (size);
	this->size = size;
	usedMemory = 0;
	numAllocations = 0;
	_current_pos = arena;
}

void* LinearAllocator::allocate(uint8_t payload, uint8_t alignment, uint8_t BoundaryFrontSize, uint8_t boundaryBackSize, uint8_t MemoryTrackingSize, uint8_t line, char* file)
{

	int headersize = this->HEADERSIZE;

	uint8_t adjustmentForward = PointerUtil::alignForwardAdjustmentWithHeader(_current_pos, alignment, headersize + BoundaryFrontSize + MemoryTrackingSize);
	uint8_t adjustmentBackward = PointerUtil::alignBackwardAdjustmentWithHeader(_current_pos, alignment, boundaryBackSize);
	uint8_t blocksize = adjustmentForward + payload + adjustmentBackward;

#ifdef _DEBUG
	if (usedMemory > size)
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
uint8_t LinearAllocator::getAllocationCount()
{
	return numAllocations;
}

void* LinearAllocator::getArena()
{
	return arena;
}

uint8_t LinearAllocator::getUsedMemory()
{
	return usedMemory;
}

uint8_t LinearAllocator::getTotalMemory()
{
	return size;
}

void LinearAllocator::deallocate(void* obj, uint8_t blocksize, uint8_t offset)
{
// there is no one ;)
}

void LinearAllocator::clear()
{
	numAllocations = 0;
	usedMemory = 0;
	_current_pos = arena;
}

LinearAllocator::~LinearAllocator()
{
	_current_pos = nullptr;
	delete arena;
}