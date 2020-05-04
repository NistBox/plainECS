#include "systems.h"
#include "components.h"

namespace ecs
{
	namespace systems
	{
		/*
			-- GravitySystem
		*/

		GravitySystem::GravitySystem()
		{
			updateType = EntityUpdate;
			typeFilter.addRequirement(components::PhysicsComponent::typeID);
			typeFilter.addRequirement(components::PositionComponent::typeID);
		}

		GravitySystem::~GravitySystem()
		{

		}

		void GravitySystem::updateEntity(FilteredEntity& _entityInfo, float _delta)
		{
			// Retrive current entity's components
			components::PhysicsComponent* physics = _entityInfo.getComponent<components::PhysicsComponent>();
			components::PositionComponent* position = _entityInfo.getComponent<components::PositionComponent>();

			// Update current entity
			physics->velocity -= 9.8 * _delta / physics->mass;
			position->height += physics->velocity * _delta;
		}



		/*
			-- GroundHitSystem
		*/

		GroundHitSystem::GroundHitSystem()
		{
			updateType = EntityUpdate;
			typeFilter.addRequirement(components::PositionComponent::typeID);
		}

		GroundHitSystem::~GroundHitSystem()
		{

		}

		void GroundHitSystem::updateEntity(FilteredEntity& _entityInfo, float _delta)
		{
			// Retrive current entity's components
			components::PositionComponent* position = _entityInfo.getComponent<components::PositionComponent>();

			// Remove entity if it has touched the ground
			if (position->height <= 0.f)
			{
				removeEntity(_entityInfo.entity->getID());
			}
		}
	}
}