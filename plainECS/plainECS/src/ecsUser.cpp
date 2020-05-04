#include "ecsUser.h"

using namespace ecs;

/*
	All ECSUser methods forward resource requests to its handler.
*/

Entity* ECSUser::getEntity(ID _entityID)
{
	return ecsUserHandler->onGetEntity(_entityID);
}

EntityIterator ECSUser::getEntitiesByFilter(TypeFilter _filter)
{

	return ecsUserHandler->onGetEntitiesByFilter(_filter);
}

EntityIterator ECSUser::getEntitiesWithComponent(TypeID _typeID)
{
	TypeFilter filter;
	filter.addRequirement(_typeID);
	return ecsUserHandler->onGetEntitiesByFilter(filter);
}

ComponentIterator ECSUser::getComponentsOfType(TypeID _typeID)
{
	return ecsUserHandler->onGetComponentsOfType(_typeID);
}

BaseComponent* ECSUser::getComponent(TypeID _typeID, ID _id)
{
	return ecsUserHandler->onGetComponent(_typeID, _id);
}

BaseComponent* ECSUser::getComponentFromKnownEntity(TypeID _typeID, ID _entityID)
{
	Entity* e = ecsUserHandler->onGetEntity(_entityID);

	// Sanity check
	if (!e || !e->hasComponentOfType(_typeID))
	{
		return nullptr;
	}

	return ecsUserHandler->onGetComponent(_typeID, e->getComponentID(_typeID));
}

Entity* ECSUser::createEntity(BaseComponent& _comp)
{
	// Create a list of component(s) and forward list to handler
	BaseComponent* components[] = { &_comp };

	ComponentList list;
	list.initialInfo = components;
	list.componentCount = 1;

	return ecsUserHandler->onCreateEntity(list);
}

Entity* ECSUser::createEntity(BaseComponent & _compA, BaseComponent & _compB)
{
	// Create a list of component(s) and forward list to handler
	BaseComponent* components[] = { &_compA, &_compB };

	ComponentList list;
	list.initialInfo = components;
	list.componentCount = 2;

	return ecsUserHandler->onCreateEntity(list);
}

Entity* ECSUser::createEntity(BaseComponent& _compA, BaseComponent& _compB, BaseComponent& _compC)
{
	// Create a list of component(s) and forward list to handler
	BaseComponent* components[] = { &_compA, &_compB, &_compC };

	ComponentList list;
	list.initialInfo = components;
	list.componentCount = 3;

	return ecsUserHandler->onCreateEntity(list);
}

Entity* ECSUser::createEntity(BaseComponent& _compA, BaseComponent& _compB, BaseComponent& _compC, BaseComponent& _compD)
{
	// Create a list of component(s) and forward list to handler
	BaseComponent* components[] = { &_compA, &_compB, &_compC, &_compD };

	ComponentList list;
	list.initialInfo = components;
	list.componentCount = 4;

	return ecsUserHandler->onCreateEntity(list);
}

Entity* ECSUser::createEntity(BaseComponent& _compA, BaseComponent& _compB, BaseComponent& _compC, BaseComponent& _compD, BaseComponent& _compE)
{
	// Create a list of component(s) and forward list to handler
	BaseComponent* components[] = { &_compA, &_compB, &_compC, &_compD, &_compE };

	ComponentList list;
	list.initialInfo = components;
	list.componentCount = 5;

	return ecsUserHandler->onCreateEntity(list);
}

Entity* ECSUser::createEntity(ComponentList _components)
{
	return ecsUserHandler->onCreateEntity(_components);
}

void* ecs::ECSUser::GetSystem(TypeID _typeID)
{
	return ecsUserHandler->onGetSystem(_typeID);
}

void ecs::ECSUser::RemoveSystem(TypeID _typeID)
{
	ecsUserHandler->onRemoveSystem(_typeID);
}

int ecs::ECSUser::getComponentCountOfType(TypeID _typeID)
{
	return ecsUserHandler->onGetComponentCountOfType(_typeID);
}

void ECSUser::createEvent(BaseEvent& _event)
{
	ecsUserHandler->onCreateEvent(_event);
}

void ECSUser::removeEntity(ID _entityID)
{
	ecsUserHandler->onRemoveEntity(_entityID);
}

void ECSUser::removeComponent(ID _entityID, TypeID _componentTypeID)
{
	ecsUserHandler->onRemoveComponent(_entityID, _componentTypeID);
}
