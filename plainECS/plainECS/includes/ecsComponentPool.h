#pragma once

#include <map>
#include <vector>
#include "ecsGlobals.h"
#include "poolAllocator.h"
#include "ecsBaseComponent.h"
#include "ecsComponentIterator.h"

namespace ecs
{
	/*
		Each component pool has a PoolAllocator, that is responsible for custom 'allocating' memory
		in a pre-allocated memory chunk and freeing of these custom 'allocations'. The ECSComponentPool
		is an abstraction of PoolAllocator, making it easier to use these for component storage.

		Each component pool holds all components of one type.
	*/
	class ECSComponentPool
	{
	public:
		// Default constructor, does not initialize a PoolAllocator. In order to use a component pool
		// that been default constructed, the component pool has to be initialized first.
		// See ECSComponentPool::initialize()
		ECSComponentPool();

		// Constructor that initializes the component pool. This makes the component pool ready to use.
		ECSComponentPool(size_t _startCap, size_t _componentSize);
		~ECSComponentPool();

		// Calls for a custom allocation in ECS memory and creates a component with given initial data,
		// then returns a base pointer to the new component.
		BaseComponent* create(BaseComponent& _component);

		// Frees given component in ECS memory.
		void remove(ID _id);

		// Returns number of components in the component pool.
		size_t getAllocations();
		BaseComponent* getComponent(ID _id);

		// Initializes a iterator for the specific component type the pool is responsible for. A component
		// iterator can only be initialized by a component pool, and can only iterate on type of component.
		ComponentIterator getIterator();

		void initialize(size_t _startCap, size_t _componentSize);

		void flagRemoval(ID _componentID);
		void removeAllFlagged();

		inline bool isInitialized()
		{
			return initialized;
		}

		inline unsigned int getCurrentRemoveFlagCount()
		{
			return (unsigned int)toRemove.size();
		}

	private:
		PoolAllocator allocator;
		std::map<ID, BaseComponent*> lookUpList;
		bool initialized = false;

		std::string componentName; // TODO: remove this
		void* memory;
		size_t memorySize;
		size_t componentCap;
		size_t componentSize;

		std::vector<ID> toRemove;
	};
}