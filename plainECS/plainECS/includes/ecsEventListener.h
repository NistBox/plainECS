#pragma once

#include "ecsEvent.h"
#include "ecsTypeFilter.h"

namespace ecs
{
	struct ECSEventListener;

	/*
		ECSEventListenerListener is a observer-type listener for ECSEventListener-objects.
		Created in order to make ECS listen on new subscriptions from systems.

		ECS includes systems, so systems can't include ECS, thus making observer pattern
		suitable for this usage.
	*/
	struct ECSEventListenerListener
	{
	protected:
		virtual void onAddSubscription(TypeID _eventTypeID, ECSEventListener* _listener) = 0;
		virtual void onRemoveSubscription(TypeID _eventTypeID, ECSEventListener* _listener) = 0;

		/*
			Virtual functions are protected. EntityComponentSystem is inheriting from
			ECSEventListenerListener, so the protection makes sure no one outside of
			EntityComponentSystem can touch these functions.
			ECSEventListener need access to the functions though, hince friending the struct.
		*/
		friend struct ECSEventListener;
	};



	/*
		ECSEventListener is an observer pattern for systems, making it possible for
		systems to be notified on event creations.
		An ECSEventListener can listen on multiple types of events, and thus has the
		responsibility to static_cast to the correct event type pointer when onEvent
		is called.

		If subscribed on multiple event types, check which event type to cast to can
		be done by following:

		events::CreateEntityEvent* pCreateEvent;
		if (_eventType == events::CreateEntityEvent::typeID)
		{
			pCreateEvent = static_cast<events::CreateEntityEvent*>(_event);
		}
	*/
	struct ECSEventListener
	{
		virtual ~ECSEventListener() {}
		// A call-back function that is called when a certain type of event is created.
		// onEvent must be overrided if the listener as a type subscription.
		virtual void onEvent(TypeID _eventType, BaseEvent* _event) {};

	protected:

		// Adds an event type that onEvent will be called-back with.
		void subscribeEventCreation(TypeID _eventType);
		void unsubscribeEventCreation(TypeID _eventType);

	private:
		TypeFilter eventTypes;
		void notifyHandler();
		TypeFilter newSubscriptions;
		TypeFilter newUnsubscriptions;
		ECSEventListenerListener* eventListenerHandler = nullptr;
		friend class EntityComponentSystem;
	};
}