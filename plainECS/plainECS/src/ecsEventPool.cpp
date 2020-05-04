#include "ecsEventPool.h"

using namespace ecs;

ECSEventPool::ECSEventPool()
{
	//
}

ecs::ECSEventPool::~ECSEventPool()
{
	clear();
}

BaseEvent* ECSEventPool::create(BaseEvent& _event)
{
	// Retrieve the event's create function.
	// This function is calling the correct constructor, whithout this
	// we don't know what constructor to call when we get a BaseEvent
	// reference.
	EventCreateFunction cf = _event.getCreateFunction();

	BaseEvent* newEvent = cf(&_event);
	events.push_back(newEvent);
	return newEvent;
}

std::vector<BaseEvent*> ECSEventPool::getIterator()
{
	return events;
}

void ECSEventPool::clear()
{
	for (BaseEvent* e : events)
	{
		// Retrieve the event's free function
		EventFreeFunction ff = e->getFreeFunction();
		ff(e);
	}
	events.clear();
}

unsigned int ecs::ECSEventPool::getEventCount()
{
	return (unsigned int)events.size();
}
