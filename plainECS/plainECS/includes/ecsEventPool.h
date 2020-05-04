#pragma once

#include <vector>
#include "ecsEvent.h"
#include "ecsEventIterator.h"

namespace ecs
{
	/*
		ECSEventPool is a handler for one type of event.
		Currently, it uses a std::vector to store events in a queue,
		later versions will use ECS memory.
	*/
	class ECSEventPool
	{
	public:
		ECSEventPool();
		~ECSEventPool();

		// Creates a new event of given type and pushes it into
		// an event queue.
		BaseEvent* create(BaseEvent& _event);
		std::vector<BaseEvent*> getIterator();

		// Frees all events and clears event queue.
		void clear();

		/*
			Getters
		*/

		unsigned int getEventCount();

	private:

		std::vector<BaseEvent*> events;
	};
}