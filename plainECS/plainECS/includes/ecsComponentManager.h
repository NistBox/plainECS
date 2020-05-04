#pragma once

#include <map>
#include "ecsGlobals.h"
#include "ecsComponent.h"
#include "ecsComponentPool.h"
#include "ecsTypeFilter.h"

#define DEFAULT_COMPONENT_POOL_SIZE_COUNT 100

namespace ecs
{
	/*
		ECSComponentManager is an abstraction of working with components.
		It's a factory for all types of components, and is responsible
		for creating and removing them using a PoolAllocator.
	*/
	class ECSComponentManager
	{
	public:
		ECSComponentManager();
		~ECSComponentManager();

		// Initializes memory for a given number of components of given type
		void initPoolType(TypeID _typeID, size_t _componentSize, size_t _count);

		/*
			Takes in component information and creates a new component in ECS memory,
			in the correct memory pool for that component's type, and then returns a
			base pointer to the new component.
		*/
		BaseComponent* createComponent(BaseComponent& _initInfo);
		BaseComponent* getComponent(TypeID _typeID, ID _id);
		void removeComponent(TypeID _typeID, ID _id);

		// Initializes a ComponentIterator for a given component TypeID and returns it.
		// The iterator only iterates one type of component.
		ComponentIterator getComponentIterator(TypeID _typeID);

		/*
			To avoid systems from accessing garbage memory, all component removals is
			handled by flagging components for removal instead. Iterators will never
			give access to components that been tagged for removal.
		*/
		void flagRemoval(TypeID _typeID, ID _componentID);

		// Function that actually clears all components from memory that have been flagged
		// for removal.
		void removeAllFlagged();

		/*
			Getters
		*/

		size_t getComponentCountOfType(TypeID _typeID);
		size_t getTotalComponentCount();
		size_t getComponentTypeCount();

		TypeFilter getInitializedComponentTypes();
		unsigned int getCurrentRemoveFlagCount();

	private:
		
		// Type used when iterating over component pools map
		using PoolPair = std::pair<TypeID, ECSComponentPool*>;

		// All existing component pools
		std::map<TypeID, ECSComponentPool*> componentPools;
	};
}