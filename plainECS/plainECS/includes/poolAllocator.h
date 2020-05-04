#pragma once

#include <memory>

// TODO: Memory alignment of where the memory should actually start.

/*
	PoolAllocator is a pure memory manager, that custom allocates memeory for objects where
	all the objects in the pool share the same size. Initialization is done by giving it a
	pointer to an pre-allocated chunk of memory, the size of the pre-allocated memory and the
	size of the	elements that will be stored in the PoolAllocator memory.
	It's important to know that the PoolAllocator only handles a given allocated memory, and
	makes custom allocations within it, thus the PoolAllocator doesn't have responsibility to
	free the pre-allocated memory.
*/
class PoolAllocator
{
public:
	PoolAllocator();
	~PoolAllocator();

	// Custom allocates a chunk of memory.
	// Returns a pointer to the newly allocated memory.
	void* allocate();

	// Custom frees a custom allocated chunk of memory.
	void free(void* _ptr);

	// Clears the memory of the PoolAllocator, sets memoryUsed to 0 and remakes the freeList.
	// If there are any objects in the PoolAllocator when this method is called, they will be
	// forgotten and the memory of them will fade away as new object will be allocated in its
	// space. Press F for respect.
	void reset();

	// A PoolAllocator can't be used before it's initialized. Initializing a PoolAllocator is
	// done by giving it a chunk of memory to work with, the size of the memory and the size
	// objects that it will custom allocate memory for. All objects within a PoolAllocators
	// memory has to share the same size.
	void initialize(void* _memoryStart, size_t _memorySize, size_t _objectSize);

	/*
		Getters
	*/

	size_t getUsedMemory();
	size_t getAllocations();

private:
	void* memoryStart;

	size_t memoryAllocations;
	size_t memorySize;
	size_t memoryUsed;
	size_t objectSize;
	size_t objectCap;

	/*
		Description on how the free list works:
		The free list is a linked list for all chunks available for allocations. This linked list
		itself is stored within the unused chunks of the PoolAllocator's memory.

		Let's us picture a freshly initialized PoolAllocator, where no allocations has been made.
		The freeList pointer will to the first memory chunk. The first memory chunk is a pointer
		to the second memory chunk. The second memory chunk is a pointer to the third memory chunk,
		and so on. The last chunk will be a nullptr, letting us know there is no more space available
		for allocations. During runtime, the freeList may not be in 'order' of memory addresses.

		As the first available memory chunk will contain a pointer, and the freeList points to it,
		the freeList is a double pointer.
	*/
	void** freeList;

	/*
		Internal help functions
	*/

	inline void* indexToPtr(size_t _index)
	{
		return (void*)((char*)memoryStart + objectSize * _index);
	}

	inline void* pointerIncrement(void* _ptr, size_t _size)
	{
		return (void*)((char*)_ptr + _size);
	}
};