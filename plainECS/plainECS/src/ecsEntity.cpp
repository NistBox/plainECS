#include "ecsEntity.h"

using namespace ecs;

ID Entity::getID()
{
	return id;
}

ID Entity::getComponentID(TypeID _typeID)
{
	// If entity has component of given type, return the component's
		// id. Else return 0.
	return (componentIDs.count(_typeID)) ? componentIDs[_typeID] : 0;
}

bool Entity::hasComponentOfType(TypeID _typeID)
{
	// If component exist, when .count return a value > 0 (==true)
	// If not exists, when .count return 0 (==false)
	return componentIDs.count(_typeID);
}

unsigned int Entity::getComponentCount()
{
	return (unsigned int)componentIDs.size();
}
