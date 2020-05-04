#pragma once

#include "ecsGlobals.h"
#include "ecsIDGenerator.h"

namespace ecs
{
	struct BaseEvent;
	typedef BaseEvent*(*EventCreateFunction)(BaseEvent* _initialData);
	typedef void(*EventFreeFunction)(BaseEvent* _eventPtr);

	/*
		BaseSystem is a top level class in the system inheritance.
		This class handles all non-type specific information about the
		system, like the list of required component type IDs.
	*/
	struct BaseEvent
	{
		virtual TypeID getTypeID() { return 0; }
		virtual std::string getName() { return ""; }
		virtual EventCreateFunction getCreateFunction() { return nullptr; }
		virtual EventFreeFunction getFreeFunction() { return nullptr; }

	protected:
		static TypeID typeIDCounter;
	};

	template <typename T>
	struct ECSEvent : public BaseEvent
	{
		static const TypeID typeID;
		static const std::string name;
		static const EventCreateFunction createFunction;
		static const EventFreeFunction freeFunction;

		virtual TypeID getTypeID() { return T::typeID; }
		virtual std::string getName() { return T::name; }
		virtual EventCreateFunction getCreateFunction() { return T::createFunction; }
		virtual EventFreeFunction getFreeFunction() { return T::freeFunction; }
	};

	/*
		Define a dynamic create and free function. This function will use the proper 
		constructor and delete operator of the actual event type pointer. This could be done
		through class and virtual destructors, but then the user might forget
		to make the destructor virtual in the user written event.
	*/
	template <typename T>
	BaseEvent* eventCreate(BaseEvent* _initialData)
	{
		return new T(*(T*)_initialData);
	}

	template <typename T>
	void eventFree(BaseEvent* _eventPtr)
	{
		T* typedPtr = (T*)_eventPtr;
		delete typedPtr;
		typedPtr = nullptr;
	}

	/*
	*	Initialize all static variables
	*/

	template <typename T>
	const TypeID ECSEvent<T>::typeID(BaseEvent::typeIDCounter++);

	template <typename T>
	const std::string ECSEvent<T>::name(__nameof<T>());

	template <typename T>
	const EventCreateFunction ECSEvent<T>::createFunction(eventCreate<T>);

	template <typename T>
	const EventFreeFunction ECSEvent<T>::freeFunction(eventFree<T>);
}