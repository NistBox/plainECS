#include "ecsEventListener.h"

using namespace ecs;

void ECSEventListener::subscribeEventCreation(TypeID _eventType)
{
	eventTypes.addRequirement(_eventType);
	newSubscriptions.addRequirement(_eventType);

	/*
		Sanity check if handler is set before nofitying it.
		This should only be rejected once, since subscription
		happens in a systems constructor and ECS sets itself as
		handler after the system's constructor has been called.

		| After ECS has set itself as handler, it will itself |
		| call ECSEventListener::notifyHandler				  |
	*/

	if (eventListenerHandler)
	{
		notifyHandler();
	}
}

void ECSEventListener::unsubscribeEventCreation(TypeID _eventType)
{
	eventTypes.removeRequirement(_eventType);
	newUnsubscriptions.addRequirement(_eventType);

	// Sanity check if handler is set before nofitying it
	if (eventListenerHandler)
	{
		notifyHandler();
	}
}

void ecs::ECSEventListener::notifyHandler()
{
	// Notify handler for all new subscriptions
	for (TypeID typeID : newSubscriptions.requirements)
	{
		eventListenerHandler->onAddSubscription(typeID, this);
	}

	// Notify handler for all new unsubscriptions
	for (TypeID typeID : newUnsubscriptions.requirements)
	{
		eventListenerHandler->onRemoveSubscription(typeID, this);
	} 

	newSubscriptions.requirements.clear();
	newUnsubscriptions.requirements.clear();
}
