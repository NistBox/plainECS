#include "poolAllocator.h"

PoolAllocator::PoolAllocator()
{
	freeList = nullptr;
	memoryStart = nullptr;
	memorySize = memoryUsed = objectSize = objectCap = memoryAllocations = 0;
}

PoolAllocator::~PoolAllocator()
{
	freeList = nullptr;
}

void* PoolAllocator::allocate()
{
	// Sanity check. If freeList is nullptr, then there aren't any more space
	// available for allocations.
	if (!freeList)
	{
		return nullptr;
	}

	// Fetch next unused chunk from free list.
	void* newAllocation = freeList;

	// Increment freeList to next unused chunk
	freeList = (void**)(*freeList);

	memoryUsed += objectSize;
	memoryAllocations++;
	return newAllocation;
}

void PoolAllocator::free(void* _ptr)
{
	// May not be necessary to set chunk memory to zero, but makes it easy
	// to detect garbage reading.
	std::memset(_ptr, 0, objectSize);
	
	// Make the now unused chunk to a pointer, pointing to the first chunk
	// in the freeList.
	*((void**)_ptr) = freeList;

	// Make the freeList point to the now unused chunk.
	freeList = (void**)_ptr;

	memoryUsed -= objectSize;
	memoryAllocations--;
}

void PoolAllocator::reset()
{
	memoryUsed = 0;
	objectCap = memorySize / objectSize;
	freeList = (void**)memoryStart;

	// Make each free chunk in the memory be a pointer to the next available
	// chunk. The free list exist in non-allocated chunks within memory.
	void** p = freeList; // Let p iterate over the memory.
	for (size_t i = 0; i < objectCap - 1; i++)
	{
		// 1. Make current p point to the next chunk
		// 2. Step to next chunk, using the pointer set in step 1.
		// 3. ...
		// 4. Profit???
		*p = pointerIncrement(p, objectSize);
		p = (void**)*p;
	}
	*p = nullptr; // Last chunk is nullptr.
}

void PoolAllocator::initialize(void* _memoryStart, size_t _memorySize, size_t _objectSize)
{
	memoryStart = _memoryStart;
	memorySize = _memorySize;
	objectSize = _objectSize;
	memoryAllocations = 0;
	reset();
}

size_t PoolAllocator::getUsedMemory()
{
	return memoryUsed;
}

size_t PoolAllocator::getAllocations()
{
	return memoryAllocations;
}
