#pragma once

#include "ecsComponentIncludes.h"

namespace ecs
{
	namespace components
	{
		struct PhysicsComponent : public ECSComponent<PhysicsComponent>
		{
			float mass;
			float velocity;
		};

		struct PositionComponent : ECSComponent<PositionComponent>
		{
			float height;
		};
	}
}