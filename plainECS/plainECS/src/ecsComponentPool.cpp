#include "ecsComponentPool.h"

using namespace ecs;

ECSComponentPool::ECSComponentPool()
{
	componentCap = 0;
	componentSize = 0;
	memorySize = 0;
	memory = nullptr;
}

ECSComponentPool::ECSComponentPool(size_t _startCap, size_t _componentSize)
{
	initialize(_startCap, _componentSize);
}

ECSComponentPool::~ECSComponentPool()
{
	if (memory)
	{
		free(memory);
		memory = nullptr;
	}
}

BaseComponent* ECSComponentPool::create(BaseComponent& _component)
{
	this->componentName = _component.getName(); // TODO: remove this
	void* p = allocator.allocate();

	if (p == nullptr)
	{
		// TODO: Add another allocator
		return 0;
	}

	ComponentCreateFunction create = _component.getCreateFunction();

	ID newID = create(p, &_component);
	if (newID == 0)
	{
		return nullptr;
	}

	lookUpList[_component.getID()] = (BaseComponent*)p;
	return (BaseComponent*)p;
}

void ECSComponentPool::remove(ID _id)
{
	// Check if id exists
	if (lookUpList.count(_id) == 0)
	{
		return;
	}

	allocator.free(lookUpList[_id]);
	lookUpList.erase(_id);
}

size_t ECSComponentPool::getAllocations()
{
	return allocator.getAllocations();
}

BaseComponent* ECSComponentPool::getComponent(ID _id)
{
	// Check if id exists
	if (lookUpList.count(_id) == 0)
	{
		return nullptr;
	}

	return lookUpList[_id];
}

ComponentIterator ECSComponentPool::getIterator()
{
	return ComponentIterator(componentSize, memorySize, memory);
}

void ECSComponentPool::initialize(size_t _startCap, size_t _componentSize)
{
	componentCap = _startCap;
	componentSize = _componentSize;
	memorySize = componentCap * componentSize;
	memory = (void*)malloc(memorySize);
	if (!memory)
	{
		return;
	}
	std::memset(memory, 0, memorySize);
	allocator.initialize(memory, memorySize, componentSize);
	initialized = true;
}

void ecs::ECSComponentPool::flagRemoval(ID _componentID)
{
	lookUpList[_componentID]->alive = false;
	toRemove.push_back(_componentID);
}

void ecs::ECSComponentPool::removeAllFlagged()
{
	while (toRemove.size())
	{
		ID id = toRemove.back();

		if (lookUpList.count(id))
		{
			BaseComponent* p_component = (BaseComponent*)lookUpList[id];

			// Fetch free function, which will call the components own destructor
			ComponentFreeFunction ff = p_component->getFreeFunction();
			ff(p_component);

			lookUpList.erase(id);
			allocator.free((void*)p_component);
			toRemove.pop_back();
		}
	}
}

ecs::ComponentIterator::ComponentIterator()
{
	first = current = nullptr;
	totalSize = objectSize = iterationSize = 0;
}

ComponentIterator::ComponentIterator(size_t _objectSize, size_t _totalSize, void* _memory)
{
	current = first = _memory;
	totalSize = _totalSize;
	objectSize = _objectSize;
	iterationSize = 0;
}

ComponentIterator::~ComponentIterator()
{
	//
}

BaseComponent* ComponentIterator::next()
{
	// Iterate in memory until next alive component is found (!alive => deleted),
	// or end of memory is reached.
	BaseComponent* pComponent;
	do
	{
		if (iterationSize >= totalSize)
		{
			return nullptr;
		}

		pComponent = (BaseComponent*)current;
		current = (void*)((char*)current + objectSize);
		iterationSize += objectSize;
	} while (!pComponent->alive);

	return pComponent;
}