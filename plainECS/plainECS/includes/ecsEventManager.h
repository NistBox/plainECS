#pragma once

#include <vector>
#include "ecsEvent.h"
#include "ecsEventPool.h"
#include "ecsEventListener.h"

namespace ecs
{
	/*
		ECSEventManager is an abstraction of working with events.
		It's a factory for all event types, and is responsible for
		both creating and removing them.
	*/
	class ECSEventManager
	{
	public:

		ECSEventManager();
		~ECSEventManager();

		// Creates a new event, pushes it back in the correct event queue and
		// notifies all interested event listeners.
		BaseEvent* createEvent(BaseEvent& _event);

		/*
			An event subscriber is an ECSEventListener, listening on one type of event.
			One ECSEventListener can listen on multiple types of events, and therefor
			has to call for subscription for each and one of the types.
		*/
		// Adds an event listener to a instant notification list if an event of their
		// subscription type is ever created.
		void addEventSubscriber(TypeID _eventTypeID, ECSEventListener* _listener);
		void removeEventSubscriber(TypeID _eventTypeID, ECSEventListener* _listener);

		std::vector<BaseEvent*> getEventIterator(TypeID _eventTypeID);

		void clearAllEvents();

		unsigned int getTotalEventCount();

	private:
		using PoolPair = std::pair<TypeID, ECSEventPool*>;
		std::map<TypeID, ECSEventPool*> eventPools;
		std::map<TypeID, std::vector<ECSEventListener*>> instantListeners;

		void notifyInstantListenersInternal(BaseEvent* _event);
	};
}