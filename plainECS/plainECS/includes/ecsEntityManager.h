#pragma once

#include <map>
#include "ecsEntity.h"
#include "ecsGlobals.h"
#include "ecsIDGenerator.h"

namespace ecs
{
	/*
		ECSEntityManager is an abstraction of working with entities.
		It's a factory for Entities, and is responsible for creating
		and removing them.
	*/
	class ECSEntityManager
	{
	public:
		ECSEntityManager();
		~ECSEntityManager();

		// Creates an empty entity and returns a pointer to it.
		Entity* createEntity();
		Entity* getEntity(ID _entityID);

		// In order to avoid reading garbage, entities are flagged for
		// removal instead and removed at a later point of time.
		// Iterators will never return an entity that is flagged for
		// removal.
		void flagRemoval(ID _entityID);
		void removeAllFlagged();

		size_t getEntityCount();
		unsigned int getCurrentRemoveFlagCount();

	private:

		// Generates a unique ID for each entity
		//IDGenerator<ID> idGenerator;
		unsigned int idCounter = 1;

		// List of all existing entitites
		std::map<ID, Entity*> entities;
		using EntityPair = std::pair<ID, Entity*>;

		// Entities flagged for removal.
		std::vector<ID> toRemove;

		 /*
			Internal help functions
		 */

		Entity* createEntityInternal();

#ifdef _DEBUG
		friend class EntityComponentSystem;
#endif
	};
}