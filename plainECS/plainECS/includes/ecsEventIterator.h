#pragma once

#include <map>
#include <vector>
#include "ecsEvent.h"
#include "ecsGlobals.h"

namespace ecs
{
	struct EventTypeIterator
	{
		using TypeList = std::map<TypeID, std::vector<BaseEvent*>>;
		using TypePair = std::pair<TypeID, std::vector<BaseEvent*>>;
		TypeList eventTypes;

		EventTypeIterator() = default;
		EventTypeIterator(const EventTypeIterator& _other)
		{
			for (TypePair list : _other.eventTypes)
			{
				eventTypes[list.first] = list.second;
			}
		}
	};
}