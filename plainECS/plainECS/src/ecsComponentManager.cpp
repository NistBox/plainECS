#include "ecsComponentManager.h"

using namespace ecs;

ECSComponentManager::ECSComponentManager()
{
	//
}

ECSComponentManager::~ECSComponentManager()
{
	for (PoolPair poolPair : componentPools)
	{
		delete poolPair.second;
	}
}

void ECSComponentManager::initPoolType(TypeID _typeID, size_t _componentSize, size_t _count)
{
	// Sanity check if pool already exist and has been initialized
	if (componentPools.count(_typeID) && componentPools[_typeID]->isInitialized())
	{
		return;
	}

	componentPools[_typeID] = new ECSComponentPool(_count, _componentSize);
	//componentPools[_typeID]->initialize(_count, _componentSize);
}

BaseComponent* ECSComponentManager::createComponent(BaseComponent& _initInfo)
{
	TypeID typeID = _initInfo.getTypeID();

	// Check if pool for component type exist. If not, create and initialize
	// one with a default size.
	if (componentPools.count(typeID) == 0 || !componentPools[typeID]->isInitialized())
	{
		componentPools[typeID] = new ECSComponentPool(DEFAULT_COMPONENT_POOL_SIZE_COUNT, _initInfo.getSize());
		//componentPools[typeID]->initialize(DEFAULT_COMPONENT_POOL_SIZE_COUNT, _initInfo.getSize());
	}
	
	return componentPools[typeID]->create(_initInfo);
}

void ECSComponentManager::removeComponent(TypeID _typeID, ID _id)
{
	// Check if memory pool for component type exists
	if (componentPools.count(_typeID) == 0)
	{
		return;
	}

	componentPools[_typeID]->remove(_id);
}

BaseComponent* ECSComponentManager::getComponent(TypeID _typeID, ID _id)
{
	// Check if pool for component type exists
	if (componentPools.count(_typeID) == 0)
	{
		return nullptr;
	}

	return componentPools[_typeID]->getComponent(_id);
}

ComponentIterator ecs::ECSComponentManager::getComponentIterator(TypeID _typeID)
{
	if (!componentPools.count(_typeID))
	{
		return ComponentIterator();
	}
	return componentPools[_typeID]->getIterator();
}

void ECSComponentManager::flagRemoval(TypeID _typeID, ID _componentID)
{
	if (!componentPools.count(_typeID))
	{
		return;
	}

	componentPools[_typeID]->flagRemoval(_componentID);
}

void ECSComponentManager::removeAllFlagged()
{
	// Iterate over all component pools and call their remove all function
	for (PoolPair poolPair : componentPools)
	{
		poolPair.second->removeAllFlagged();
	}
}

size_t ECSComponentManager::getComponentCountOfType(TypeID _typeID)
{
	// If pool exist, return number of components in that pool. Else return 0.
	return componentPools.count(_typeID) ? componentPools[_typeID]->getAllocations() : 0;
}

size_t ECSComponentManager::getTotalComponentCount()
{
	// TODO: subtract with current remove flags?
	size_t sum = 0;
	for (PoolPair poolPair : componentPools)
	{
		sum += poolPair.second->getAllocations();
	}

	return sum;
}

size_t ECSComponentManager::getComponentTypeCount()
{
	return componentPools.size();
}

TypeFilter ecs::ECSComponentManager::getInitializedComponentTypes()
{
	TypeFilter filter;
	for (PoolPair poolPair : componentPools)
	{
		filter.addRequirement(poolPair.first);
	}
	return filter;
}

unsigned int ECSComponentManager::getCurrentRemoveFlagCount()
{
	unsigned int sum = 0;
	for (PoolPair pair : componentPools)
	{
		sum += pair.second->getCurrentRemoveFlagCount();
	}

	return sum;
}
