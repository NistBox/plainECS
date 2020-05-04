#pragma once

#include "ecsSystemIncludes.h"

namespace ecs
{
	namespace systems
	{
		class GravitySystem : public ecs::ECSSystem<GravitySystem>
		{
		public:
			GravitySystem();
			~GravitySystem();

			void updateEntity(FilteredEntity& _entityInfo, float _delta) override;
		};

		class GroundHitSystem : public ecs::ECSSystem<GroundHitSystem>
		{
		public:
			GroundHitSystem();
			~GroundHitSystem();

			void updateEntity(FilteredEntity& _entityInfo, float _delta) override;
		};
	}
}