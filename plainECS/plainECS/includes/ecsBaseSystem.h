#pragma once

#include <vector>
#include <unordered_map>
#include "ecsUser.h"
#include "ecsEventListener.h"
#include "ecsEntity.h"
#include "ecsEventIterator.h"
#include "ecsGlobals.h"
#include "ecsIDGenerator.h"
#include "ecsTypeFilter.h"



namespace ecs
{
	class BaseSystem;
	typedef BaseSystem* (*SystemCreateFunction)();
	typedef void(*SystemFreeFunction)(BaseSystem* _systemPtr);



	/*
		SystemUpdateType description
			- Undefined				Unset update type
			- EntityUpdate			Updates ONE entity per update call, ECS will loop all filtered
									entities until the system have updated all of them.
			- MultiEntityUpdate		Updates ALL entities of interest per update call. The system
									will recieve an EntityIterator and will have full responsibility
									of iterating through the given iterator of filtered entities.
			- EventReader			System will reviece ONE event and the event TypeID per update.
									The system have responsibility of casting the given BaseEvent*
									to the wanted event type.
			- EventListenerOnly		The system will ONLY get updated when an event is created, recieving
									a BaseEvent* and the event TypeID. The system have responsibility of
									casting the event pointer to wanted event type and act upon it.
			- Actor					The system will be updated without any entity or event input.
	*/

	// Defines the wanted updated behaviour of a custom system.
	enum SystemUpdateType { Undefined, EntityUpdate, MultiEntityUpdate, EventReader, EventListenerOnly, Actor };

	/*
		BaseSystem is a top level class in the system inheritance.
		This class handles all non-type specific information about the
		system, like the list of required component type IDs.
	*/
	class BaseSystem : public ECSUser, public ECSEventListener
	{
	public:
		BaseSystem() {}
		virtual ~BaseSystem() { updateType = Undefined; }

		virtual TypeID getTypeID() { return 0; }
		virtual std::string getName() { return ""; }
		virtual SystemCreateFunction getCreateFunction() { return nullptr; }
		virtual SystemFreeFunction getFreeFunction() { return nullptr; }

		virtual void updateEntity(FilteredEntity &_entityInfo, float _delta) {}
		virtual void updateMultipleEntities(EntityIterator &_entities, float _delta) {}
		virtual void readEvent(BaseEvent &_event, float _delta) {}
		virtual void act(float _delta) {}

	protected:

		SystemUpdateType updateType = Undefined;
		//TypeFilter componentFilter;
		//TypeFilter eventFilter;
		TypeFilter typeFilter;

		//static std::unordered_map<TypeID, SystemCreateFunction>* typeIDToCreateFunctionMap;

		//static SystemCreateFunction RegisterCreateFunction(TypeID _typeID, SystemCreateFunction cf);

		//static IDGenerator<TypeID> typeIDGenerator;
		static TypeID typeIDCounter;
		friend class EntityComponentSystem;
	};

	/*
		ECSSystem is the class all user systems will inherit from.
		This class handles all type specific information, like type ID.
	*/
	template <typename T>
	class ECSSystem : public BaseSystem
	{
	public:
		static const TypeID typeID;
		static const std::string name;
		static const SystemCreateFunction createFunction;
		static const SystemFreeFunction freeFunction;

		virtual TypeID getTypeID() { return T::typeID; }
		virtual std::string getName() { return T::name; }
		virtual SystemCreateFunction getCreateFunction() { return T::createFunction; }
		virtual SystemFreeFunction getFreeFunction() { return T::freeFunction; }

		ECSSystem() {}
		virtual ~ECSSystem() {}
	};

	/*
		Define a dynamic create and free function. This functions will use the proper
		construct and delete operator of the actual system type pointer. This could be done
		through virtual destructors, but then the user might forget
		to make the destructor virtual in the user written systems.
	*/
	template <typename T>
	BaseSystem* systemCreate()
	{
		return (BaseSystem*)(new T());
	}

	template <typename T>
	void systemFree(BaseSystem* _systemPtr)
	{
		T* typedPtr = (T*)_systemPtr;
		delete typedPtr;
		typedPtr = nullptr;
	}

	/*
	*	Initialize all static variables and functions
	*/

	template <typename T>
	const TypeID ECSSystem<T>::typeID(BaseSystem::typeIDCounter++);

	template <typename T>
	const std::string ECSSystem<T>::name(__nameof<T>());

	template <typename T>
	const SystemCreateFunction ECSSystem<T>::createFunction(systemCreate<T>);
	//const SystemCreateFunction ECSSystem<T>::createFunction(systemCreate<T>);

	template <typename T>
	const SystemFreeFunction ECSSystem<T>::freeFunction(systemFree<T>);
}