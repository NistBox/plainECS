#include "ecsEventManager.h"

using namespace ecs;

ECSEventManager::ECSEventManager()
{
	//
}

ECSEventManager::~ECSEventManager()
{
	for (PoolPair pool : eventPools)
	{
		pool.second->clear();
		delete pool.second;
	}
	eventPools.clear();
}

BaseEvent* ECSEventManager::createEvent(BaseEvent& _event)
{
	TypeID typeID = _event.getTypeID();

	// Sanity check memory pool exist
	if (!eventPools.count(typeID))
	{
		eventPools[typeID] = new ECSEventPool;
	}

	BaseEvent* pNewEvent = eventPools[typeID]->create(_event);

	notifyInstantListenersInternal(pNewEvent);
	return pNewEvent;
}

void ECSEventManager::addEventSubscriber(TypeID _eventTypeID, ECSEventListener* _listener)
{
	// Check if listener already exist
	std::vector<ECSEventListener*>& list = instantListeners[_eventTypeID];
	for (ECSEventListener* l : list)
	{
		if (l == _listener)
		{
			return;
		}
	}

	instantListeners[_eventTypeID].push_back(_listener);
}

void ECSEventManager::removeEventSubscriber(TypeID _eventTypeID, ECSEventListener* _listener)
{
	// Sanity check if list exist
	if (!instantListeners.count(_eventTypeID))
	{
		return;
	}

	// Iterate over all listeners, and erasing the listener when found
	std::vector<ECSEventListener*>& list = instantListeners[_eventTypeID];
	for (size_t i = 0; i < list.size(); i++)
	{
		if (list[i] == _listener)
		{
			list.erase(list.begin() + i);
			return;
		}
	}
}

std::vector<BaseEvent*> ECSEventManager::getEventIterator(TypeID _eventTypeID)
{
	// Sanity check event pool exist
	if (!eventPools.count(_eventTypeID))
	{
		return std::vector<BaseEvent*>();
	}

	return eventPools[_eventTypeID]->getIterator();
}

void ECSEventManager::clearAllEvents()
{
	// Iterate over all event pools and clear them
	for (PoolPair pair : eventPools)
	{
		pair.second->clear();
	}
}

unsigned int ecs::ECSEventManager::getTotalEventCount()
{
	unsigned int sum = 0;
	for (PoolPair pair : eventPools)
	{
		sum += pair.second->getEventCount();
	}
	return sum;
}

void ECSEventManager::notifyInstantListenersInternal(BaseEvent* _event)
{
	TypeID typeID = _event->getTypeID();

	// Check if list exist
	if (!instantListeners.count(typeID))
	{
		return;
	}

	std::vector<ECSEventListener*>& list = instantListeners[typeID];
	for (ECSEventListener* l : list)
	{
		l->onEvent(typeID, _event);
	}
}
