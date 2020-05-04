#include "ecsTypeFilter.h"

using namespace ecs;

TypeFilter::TypeFilter()
{
	//
}

TypeFilter::~TypeFilter()
{
	//
}

void TypeFilter::addRequirement(TypeID _typeID)
{
	// Check if requirement already exists
	for (TypeID t : this->requirements)
	{
		if (t == _typeID)
		{
			return;
		}
	}
	this->requirements.push_back(_typeID);
}

void TypeFilter::removeRequirement(TypeID _typeID)
{
	for (size_t i = 0; i < this->requirements.size(); i++)
	{
		if (this->requirements[i] == _typeID)
		{
			this->requirements.erase(this->requirements.begin() + i);
			return;
		}
	}
}

std::vector<TypeID> ecs::TypeFilter::getRequirements()
{
	return this->requirements;
}
