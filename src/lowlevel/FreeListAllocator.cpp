#include "precompiled.h"
#include "FreeListAllocator.h"

FreeListAllocator::FreeListAllocator(size_t size)
{
	arena = operator new (size);
	this->size = size;
	usedMemory = 0;
	numAllocations = 0;

	free_blocks = (FreeBlock*)arena;
	free_blocks->size = size;
	free_blocks->next = nullptr;

}

void* FreeListAllocator::allocate(size_t blocksize, size_t alignment, size_t offset, size_t boundaryBackSize, size_t line, char* file)
{
	
	
	return nullptr;
}

size_t FreeListAllocator::getAllocationCount()
{
	return numAllocations;
}

void* FreeListAllocator::getArena()
{
	return arena;
}

size_t FreeListAllocator::getUsedMemory()
{
	return usedMemory;
}

size_t FreeListAllocator::getTotalMemory()
{
	return size;
}

void FreeListAllocator::deallocate(void* obj, size_t blocksize, size_t alignment, size_t offset)
{

}

void FreeListAllocator::clear()
{
	numAllocations = 0;
	usedMemory = 0;
	_current_pos = arena;
}

FreeListAllocator::~FreeListAllocator()
{
	_current_pos = nullptr;
	free_blocks = nullptr;
	delete arena;
}