#pragma once

#include <vector>
#include "ecsGlobals.h"

namespace ecs
{
	/*
		TypeFilter is a list of types, used for filtering either
		components or events.
	*/
	class TypeFilter
	{
	public:
		TypeFilter();
		~TypeFilter();

		void addRequirement(TypeID _typeID);
		void removeRequirement(TypeID _typeID);

		std::vector<TypeID> getRequirements();

	private:
		std::vector<TypeID> requirements;
		friend class EntityComponentSystem;
		friend struct ECSEventListener;
	};
}