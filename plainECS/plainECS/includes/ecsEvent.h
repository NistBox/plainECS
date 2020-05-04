#pragma once

#include <map>
#include "ecsBaseEvent.h"
#include "ecsGlobals.h"

namespace ecs
{
	namespace events
	{
		struct CreateEntityEvent : public ECSEvent<CreateEntityEvent>
		{
			ID entityID = 0;
		};

		struct RemoveEntityEvent : public ECSEvent<RemoveEntityEvent>
		{
			ID entityID = 0;
		};

		struct CreateComponentEvent : public ECSEvent<CreateComponentEvent>
		{
			ID entityID = 0;
			ID componentID = 0;
			TypeID componentTypeID = 0;
		};

		struct RemoveComponentEvent : public ECSEvent<RemoveComponentEvent>
		{
			ID entityID = 0;
			ID componentID = 0;
			TypeID componentTypeID = 0;
		};
	}
}