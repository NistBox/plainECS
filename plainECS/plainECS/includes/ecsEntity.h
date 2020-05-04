#pragma once

#include <map>
#include <vector>
#include "ecsGlobals.h"
#include "ecsBaseComponent.h"

namespace ecs
{
	/*
		Entity is a collection of component IDs, and 'getters' to retrieve these IDs.
		Each entity has a unique ID.
	*/
	struct Entity
	{
		ID getID();

		/*
			Non-templated getters
		*/

		// An entity can only have up to one component per type,
		// thus making component type ID enough to fetch component ID.
		ID getComponentID(TypeID _typeID);
		bool hasComponentOfType(TypeID _typeID);
		unsigned int getComponentCount();

		/*
			Templated getters
		*/

		// Example usage:
		// (Entity*)->getComponentID<components::HealthComponent>();
		template <typename T> ID getComponentID();

		// Example usage:
		// (Entity*)->hasComponentOfType<components::HealthComponent>();
		template <typename T> bool hasComponentOfType();

	private:
		ID id;
		std::map<TypeID, ID> componentIDs;

		friend class ECSEntityManager;
		friend class EntityComponentSystem;
	};

	/*
		FilterentEntity contains pointers to an entity and all of its components
		that has been requested. This way, system updates doesn't need pointers
		to all components the entity has. They only sees the components it is
		interested in.
	*/
	struct FilteredEntity
	{
		Entity* entity = nullptr;
		std::map<TypeID, BaseComponent*> components;

		template <typename T>
		T* getComponent() { return (components.count(T::typeID)) ? (T*)components[T::typeID] : nullptr; }
	};

	/*
		EntityIterator is a collection of filtered entities.
		All entities in a iterator are defined by the same ComponentFilter,
		so it can be assumed that all the entities at least has the
		components of the requested types.
	*/
	struct EntityIterator
	{
		std::vector<FilteredEntity> entities;
	};


	/*
	*	Templated functions
	*/

	template<typename T>
	inline ID Entity::getComponentID()
	{
		// If entity has component of given type, return the component's
		// id. Else return 0.
		return (componentIDs.count(T::typeID)) ? componentIDs[T::typeID] : 0;
	}

	template<typename T>
	inline bool Entity::hasComponentOfType()
	{
		// If component exist, when .count return a value > 0 (==true)
		// If not exists, when .count return 0 (==false)
		return componentIDs.count(T::typeID);
	}
}