#include "ecs.h"

using namespace ecs;

EntityComponentSystem::EntityComponentSystem()
{
	layerCount = 0;
	systemLayers = nullptr;
}

EntityComponentSystem::~EntityComponentSystem()
{
	// Loop through all layers
	for (size_t i = 0; i < layerCount; i++)
	{
		// Loop through all systems in layer
		for (BaseSystem* s : systemLayers[i])
		{
			// Fetch free function. This function uses the real destructor of the system.
			// This is the same as if the system has a virtual destructor, but just in case
			// the user hasn't set the system's destructor as virtual.
			if (s)
			{
				/*SystemFreeFunction ff = s->getFreeFunction();
				ff(s);*/
				delete s;
			}
		}
		systemLayers[i].clear();
	}
	delete[] systemLayers;
}

bool ecs::EntityComponentSystem::initialize(ECSDesc& _desc)
{
	// Init system layers
	layerCount = _desc.systemLayerCount;
	systemLayers = new SystemList[layerCount];

	// Init component memories
	CompTypeMemDesc desc;
	for (size_t i = 0; i < _desc.compTypeCount; i++)
	{
		desc = _desc.compTypeMemDescs[i];
		componentMgr.initPoolType(desc.typeID, desc.componentSize, desc.memoryCount);
	}

	return true;
}

Entity* EntityComponentSystem::createEntity(BaseComponent& _comp)
{
	/*
	*	Creates a component list and forwards creation to internal function.
	*	See createEntityInternal for further information.
	*/

	BaseComponent* components[] = { &_comp };

	ComponentList list;
	list.initialInfo = components;
	list.componentCount = 1;

	return createEntityInternal(list);
}

Entity* EntityComponentSystem::createEntity(BaseComponent& _compA, BaseComponent& _compB)
{
	/*
	*	Creates a component list and forwards creation to internal function.
	*	See createEntityInternal for further information.
	*/

	BaseComponent* components[] = { &_compA, &_compB };

	ComponentList list;
	list.initialInfo = components;
	list.componentCount = 2;

	return createEntityInternal(list);
}

Entity* EntityComponentSystem::createEntity(BaseComponent& _compA, BaseComponent& _compB, BaseComponent& _compC)
{
	/*
	*	Creates a component list and forwards creation to internal function.
	*	See createEntityInternal for further information.
	*/

	BaseComponent* components[] = { &_compA, &_compB, &_compC };

	ComponentList list;
	list.initialInfo = components;
	list.componentCount = 3;

	return createEntityInternal(list);
}

Entity* EntityComponentSystem::createEntity(BaseComponent& _compA, BaseComponent& _compB, BaseComponent& _compC, BaseComponent& _compD)
{
	/*
	*	Creates a component list and forwards creation to internal function.
	*	See createEntityInternal for further information.
	*/

	BaseComponent* components[] = { &_compA, &_compB, &_compC, &_compD };

	ComponentList list;
	list.initialInfo = components;
	list.componentCount = 4;

	return createEntityInternal(list);
}

Entity* EntityComponentSystem::createEntity(BaseComponent& _compA, BaseComponent& _compB, BaseComponent& _compC, BaseComponent& _compD, BaseComponent& _compE)
{
	/*
	*	Creates a component list and forwards creation to internal function.
	*	See createEntityInternal for further information.
	*/

	BaseComponent* components[] = { &_compA, &_compB, &_compC, &_compD, &_compE };

	ComponentList list;
	list.initialInfo = components;
	list.componentCount = 5;

	return createEntityInternal(list);
}

Entity* EntityComponentSystem::createEntity(ComponentList _components)
{
	// Forwards to internal function.
	return createEntityInternal(_components);
}

void EntityComponentSystem::removeEntity(ID _entityID)
{
	// Forwards to internal function.
	removeEntityInternal(_entityID);
}

void EntityComponentSystem::removeComponent(ID _entityID, TypeID _componentTypeID)
{
	// Forwards to internal function.
	removeComponentInternal(_entityID, _componentTypeID);
}

void EntityComponentSystem::update(float _delta)
{
	/*
	*	This is the MAIN update function that handles all ECS system updates.
	*
	*	Systems update priority are defined in layers, where the first layer
	*	contains the highest priority system updates and the last layer contains
	*	the lowest priority system updates. This function loops through the layers,
	*	updating all system within that layer and then continues to the next layer.
	*
	*
	*	SystemUpdateType description:
	*		- Undefined				Unset update type
	*		- EntityUpdate			Updates ONE entity per update call, ECS will loop all filtered
	*								entities until the system have updated all of them.
	*		- MultiEntityUpdate		Updates ALL entities of interest per update call. The system
	*								will recieve an EntityIterator and will have full responsibility
	*								of iterating through the given iterator of filtered entities.
	*		- EventReader			System will reviece ONE event and the event TypeID per update.
	*								The system have responsibility of casting the given BaseEvent*
	*								to the wanted event type.
	*		- EventListenerOnly		The system will ONLY get updated when an event is created, recieving
	*								a BaseEvent* and the event TypeID. The system have responsibility of
	*								casting the event pointer to wanted event type and act upon it.
	*		- Actor					The system will be updated without any entity or event input.
	*/

	//#ifdef _DEBUG
	//	std::string debugPrint = "\n[" + __nameof<EntityComponentSystem>() + "]\t Updating systems:\n";
	//#endif

	/*
	*	Iterate all layers
	*/

	EntityIterator entities;
	EventTypeIterator events;
	for (size_t i = 0; i < layerCount; i++)
	{
		/*
		*	Iterate all system within this layer
		*/

		SystemList& layer = systemLayers[i];

		//#ifdef _DEBUG
		//	// Add layer print
		//	if ((unsigned int)layer.size())
		//	{
		//		debugPrint += "\t[layer " + to_string(i) + "] ";
		//	}
		//#endif

		for (BaseSystem* s : layer)
		{
			//#ifdef _DEBUG
			//	// Print system
			//	debugPrint += s->getName();
			//#endif

			/*
			*	Fetch the system's update behaviour and act accordingly.
			*/

			switch (s->updateType)
			{
			case EntityUpdate:
				// Fill list with entities that own all of the system's
				// required component types.
				fillEntityIteratorInternal(s->typeFilter, entities);

				// Iterate through all the entites of interest and update the system.
				for (FilteredEntity e : entities.entities)
				{
					s->updateEntity(e, _delta);
				}
				break;

			case MultiEntityUpdate:
				// Fill list with entities that own all of the system's
				// required component types.
				fillEntityIteratorInternal(s->typeFilter, entities);

				// Update the system with the list of all entities of interest.
				s->updateMultipleEntities(entities, _delta);
				break;

			case EventReader:
				// Fill list with events of the system's interest
				fillEventIteratorInternal(s->typeFilter, events);

				// Iterator through all event types
				for (EventTypeIterator::TypePair list : events.eventTypes)
				{
					// Iterates through all events of current type
					for (BaseEvent* e : list.second)
					{
						s->readEvent(*e, _delta);
					}
				}
				break;
			case Actor:
				s->act(_delta);
				break;

		//#ifdef _DEBUG
		//	case Undefined:
		//		debugPrint += "(Undefined update type)";
		//		break;
		//#endif

			}

			//#ifdef _DEBUG
			//	debugPrint += ", ";
			//#endif
		}

		//#ifdef _DEBUG
		//	if ((unsigned int)layer.size())
		//	{
		//		debugPrint += "\n";
		//	}
		//#endif
	}

	/*
	*	All removals during the update loop are only flagged, the real removal
	*	happends after all system has been updated. This is for safety, since
	*	a system may otherwise read memory garbage. Iterators never return an
	*	item that has been flagged for removal.
	*
	*	Event's lifetime is only until the end of an ECS update.
	*/

	componentMgr.removeAllFlagged();
	entityMgr.removeAllFlagged();
	eventMgr.clearAllEvents();

//#ifdef _DEBUG
//	debugPrint += "\tComp. count:\t" + to_string(componentMgr.getTotalComponentCount()) + "\n";
//	debugPrint += "\tEntity count:\t" + to_string(entityMgr.getEntityCount()) + "\n";
//
//	if (entityMgr.getEntityCount() <= DEBUG_ENTITY_PRINT_MAX_COUNT)
//	{
//		unsigned int counter = 1;
//		using IDPair = std::pair<TypeID, ID>;
//		for (ECSEntityManager::EntityPair e : entityMgr.entities)
//		{
//			debugPrint += "\t  ";
//
//			debugPrint += (counter != entityMgr.getEntityCount()) ? "|" : " ";
//
//			debugPrint += "`-[ID=" + to_string(e.second->getID()) + "] ";
//
//			
//			for (IDPair idPair : e.second->componentIDs)
//			{
//				BaseComponent* pComp = getComponent(idPair.first, idPair.second);
//				debugPrint += pComp->getName() + " ";
//			}
//			debugPrint += "\n";
//			counter++;
//		}
//	}
//#endif

//#ifdef _DEBUG
//	cout << debugPrint << endl;
//#endif
}

/*
*	Getters
*/

Entity* EntityComponentSystem::getEntity(ID _id)
{
	return entityMgr.getEntity(_id);
}

BaseComponent* EntityComponentSystem::getComponent(TypeID _typeID, ID _id)
{
	return componentMgr.getComponent(_typeID, _id);
}

BaseComponent* EntityComponentSystem::getComponentFromEntity(TypeID _typeID, ID _entityID)
{
	Entity* entity = entityMgr.getEntity(_entityID);

	// Sanity check entity creation
	if (!entity->hasComponentOfType(_typeID))
	{
		return nullptr;
	}

	return componentMgr.getComponent(_typeID, entity->getComponentID(_typeID));
}

size_t EntityComponentSystem::getSystemLayerCount()
{
	return layerCount;
}

size_t EntityComponentSystem::getTotalSystemCount()
{
	return typeIDToLayerMap.size();
}

size_t EntityComponentSystem::getTotalEntityCount()
{
	return entityMgr.getEntityCount();
}

size_t EntityComponentSystem::getTotalComponentCount()
{
	return componentMgr.getTotalComponentCount();
}

size_t EntityComponentSystem::getComponentTypeCount()
{
	return componentMgr.getComponentTypeCount();
}

size_t EntityComponentSystem::getComponentCountOfType(TypeID _typeID)
{
	return componentMgr.getComponentCountOfType(_typeID);
}

EntityIterator EntityComponentSystem::getEntititesByFilter(TypeFilter _componentFilter)
{
	EntityIterator iterator;
	fillEntityIteratorInternal(_componentFilter, iterator);
	return iterator;
}

ComponentIterator EntityComponentSystem::getAllComponentsOfType(TypeID _typeID)
{
	return componentMgr.getComponentIterator(_typeID);
}

TypeFilter EntityComponentSystem::getInitializedComponentTypes()
{
	return componentMgr.getInitializedComponentTypes();
}



/*
*	Internal functionality
*/

Entity* EntityComponentSystem::onGetEntity(ID _entityID)
{
	return entityMgr.getEntity(_entityID);
}

BaseComponent* EntityComponentSystem::onGetComponent(TypeID _typeID, ID _id)
{
	return componentMgr.getComponent(_typeID, _id);
}

Entity* EntityComponentSystem::onCreateEntity(ComponentList _components)
{
	return createEntityInternal(_components);
}

BaseComponent* EntityComponentSystem::onCreateComponent(ID _entityID, BaseComponent& _componentInfo)
{
	return createComponentInternal(_entityID, _componentInfo);
}

EntityIterator EntityComponentSystem::onGetEntitiesByFilter(TypeFilter _componentFilter)
{
	EntityIterator iterator;
	fillEntityIteratorInternal(_componentFilter, iterator);
	return iterator;
}

ComponentIterator EntityComponentSystem::onGetComponentsOfType(TypeID _typeID)
{
	return componentMgr.getComponentIterator(_typeID);
}

void EntityComponentSystem::onCreateEvent(BaseEvent& _event)
{
	createEventInternal(_event);
}

void EntityComponentSystem::onRemoveEntity(ID _entityID)
{
	removeEntityInternal(_entityID);
}

void EntityComponentSystem::onRemoveComponent(ID _entityID, TypeID _componentTypeID)
{
	removeComponentInternal(_entityID, _componentTypeID);
}

void* ecs::EntityComponentSystem::onGetSystem(TypeID _typeID)
{
	if (!typeIDToLayerMap.count(_typeID))
	{
		return nullptr;
	}

	unsigned int layer = typeIDToLayerMap[_typeID];

	for (BaseSystem* s : systemLayers[layer])
	{
		if (s->getTypeID() == _typeID)
		{
			return (void*)s;
		}
	}

	return nullptr;
}

void* ecs::EntityComponentSystem::onCreateSystem(void* _tempSystem, unsigned int _layer)
{
	BaseSystem* pTempSystemInfo = (BaseSystem*)_tempSystem;

	SystemCreateFunction cf = pTempSystemInfo->getCreateFunction();

	// Check if systemLayers been initialized
	if (!systemLayers)
	{
		layerCount = 10;
		systemLayers = new SystemList[10];
	}

	// Check if system is already added,
	// should only exist one system at most per type
	if (typeIDToLayerMap.count(pTempSystemInfo->getTypeID()) != 0)
	{
		return nullptr;
	}

	/*
	*	Create system, which calls its constructor.
	*	The system's constructor will set
	*		- UpdateType
	*		- EventSubscriptions (added to list, read later when ECS calls for it
	*		- Component/Event filter
	*/
	BaseSystem* newSystem = cf();

	// Make ECS listen on ECSUser functionality. This makes ECS responsible for
	// handling entity creations, component removals etc.
	(static_cast<ECSUser*>(newSystem))->ecsUserHandler = this;

	// Set ECS to handle all subscriptions and unsubscriptions on event creations.
	ECSEventListener* listenerCast = static_cast<ECSEventListener*>(newSystem);

	// Fetch all subscriptions. This must be done AFTER eventListenerHandler been set,
	// as it will call eventListerHandler's onAddSubscription-overrided function.
	listenerCast->eventListenerHandler = this;
	listenerCast->notifyHandler();

	// Set system in hashed list for easy access
	typeIDToLayerMap[pTempSystemInfo->getTypeID()] = _layer;

	// Push back system in wanted layer for later update
	systemLayers[_layer].push_back(newSystem);
	return (void*)newSystem;
}

void ecs::EntityComponentSystem::onRemoveSystem(TypeID _typeID)
{
	if (!typeIDToLayerMap.count(_typeID))
	{
		return;
	}
	SystemList& layer = systemLayers[typeIDToLayerMap[_typeID]];

	BaseSystem* systemPtr = nullptr;
	for (size_t i = 0; i < layer.size(); i++)
	{
		systemPtr = layer[i];	// Store system pointer

		if (systemPtr->getTypeID() == _typeID)
		{

			layer.erase(layer.begin() + i);	 // Remove system from layer
			delete systemPtr;				 
			typeIDToLayerMap.erase(_typeID); // Erase from hash map

			return;
		}
	}
}

int ecs::EntityComponentSystem::onGetComponentCountOfType(TypeID _typeID)
{
	return componentMgr.getComponentCountOfType(_typeID);
}

void EntityComponentSystem::onAddSubscription(TypeID _eventTypeID, ECSEventListener* _listener)
{
	eventMgr.addEventSubscriber(_eventTypeID, _listener);
}

void EntityComponentSystem::onRemoveSubscription(TypeID _eventTypeID, ECSEventListener* _listener)
{
	eventMgr.removeEventSubscriber(_eventTypeID, _listener);
}

Entity* EntityComponentSystem::createEntityInternal(ComponentList _components)
{
	Entity* entity = entityMgr.createEntity();

	/*
	*	Create all components and add them to the entity
	*/

	BaseComponent* component;
	for (size_t i = 0; i < _components.componentCount; i++)
	{
		component = createComponentInternal(entity->getID(), *_components.initialInfo[i]);

		// Sanity check component creation
		if (!component)
		{
			// Remove entity if not all components can be created
			removeEntityInternal(entity->getID());
			return nullptr;
		}

		// Make entity know of component
		entity->componentIDs[component->getTypeID()] = component->getID();
	}

	// Create event
	events::CreateEntityEvent e;
	e.entityID = entity->getID();
	BaseEvent* pEvent = eventMgr.createEvent(e);

	return entity;
}

BaseComponent* EntityComponentSystem::createComponentInternal(ID _entityID, BaseComponent& _componentInfo)
{
	// Fetch entity
	Entity* entity = entityMgr.getEntity(_entityID);

	// Sanity check that entity exist
	if (!entity)
	{
		return nullptr;
	}

	// Create component
	_componentInfo.entityID = _entityID;
	BaseComponent* component = componentMgr.createComponent(_componentInfo);

	// Sanity check component creation
	if (!component)
	{
		return nullptr;
	}

	entity->componentIDs[component->getTypeID()] = component->getID();

	// Create event
	events::CreateComponentEvent e;
	e.entityID = entity->getID();
	e.componentID = component->getID();
	e.componentTypeID = component->getTypeID();
	BaseEvent* pEvent = eventMgr.createEvent(e);

	return component;
}

void EntityComponentSystem::createEventInternal(BaseEvent& _event)
{
	eventMgr.createEvent(_event);
}

void EntityComponentSystem::removeEntityInternal(ID _entityID)
{
	Entity* entity = entityMgr.getEntity(_entityID);

	// Sanity check entity exist
	if (!entity)
	{
		return;
	}

	std::map<TypeID, ID>::iterator it;
	for (std::pair<TypeID, ID> c : entity->componentIDs)
	{
		events::RemoveComponentEvent rve;
		rve.entityID = _entityID;
		rve.componentID = c.second;
		rve.componentTypeID = c.first;
		eventMgr.createEvent(rve);
		componentMgr.flagRemoval(c.first, c.second);
	}
	entity->componentIDs.clear();

	events::RemoveEntityEvent ree;
	ree.entityID = _entityID;
	eventMgr.createEvent(ree);
	entityMgr.flagRemoval(_entityID);
}

void EntityComponentSystem::removeComponentInternal(ID _entityID, TypeID _componentTypeID)
{
	Entity* entity = entityMgr.getEntity(_entityID);
	if (!entity)
	{
		return;
	}

	ID componentID = entity->getComponentID(_componentTypeID);
	if (!componentID)
	{
		return;
	}

	// Create event and notify event listeners before removal
	events::RemoveComponentEvent rve;
	rve.entityID = _entityID;
	rve.componentID = componentID;
	rve.componentTypeID = _componentTypeID;
	eventMgr.createEvent(rve);

	// Erasing component ID from entity must happen after event is created,
	// in case some systems need to act to event.
	entity->componentIDs.erase(_componentTypeID);
	componentMgr.flagRemoval(_componentTypeID, componentID);

	// If the entity only had one component, and is now down to zero components,
	// remove the entity.
	if (!entity->componentIDs.size())
	{
		removeEntityInternal(_entityID);
	}
}

void EntityComponentSystem::fillEntityIteratorInternal(TypeFilter& _componentFilter, EntityIterator& _iterator)
{
	// Reset iterator from junk
	_iterator.entities.clear();

	// Get component pool with the least amount of allocations
	TypeID minTypeID = 0;
	size_t minSize = 0;
	for (TypeID typeID : _componentFilter.requirements)
	{
		size_t size = componentMgr.getComponentCountOfType(typeID);
		if (size < minSize || minTypeID == 0)
		{
			minTypeID = typeID;
			minSize = size;
		}
	}

	// Get iterator from pool
	ComponentIterator firstReqTypeIterator = componentMgr.getComponentIterator(minTypeID);

	// If single component type requirement
	BaseComponent* pComponent;
	if (_componentFilter.requirements.size() == 1)
	{
		while (pComponent = firstReqTypeIterator.next())
		{
			FilteredEntity info;
			info.entity = entityMgr.getEntity(pComponent->getEntityID());
			info.components[minTypeID] = pComponent;
			_iterator.entities.push_back(info);
		}
	}
	else // If multiple component type requirement
	{
		while (pComponent = firstReqTypeIterator.next())
		{
			
			Entity* pEntity = entityMgr.getEntity(pComponent->getEntityID());

			if (!pEntity)
			{
				componentMgr.flagRemoval(pComponent->getTypeID(), pComponent->getID());
				continue;
			}

			FilteredEntity info;
			bool entityIsValid = true;

			// Fill EntityInfo with component pointers.
			// If encountering a component type that the entity does not have,
			// skip this EntityInfo and start checking next entity.
			for (TypeID typeID : _componentFilter.requirements)
			{
				if (!pEntity->hasComponentOfType(typeID))
				{
					entityIsValid = false;
					break;
				}
				info.components[typeID] = componentMgr.getComponent(typeID, pEntity->getComponentID(typeID));
			}

			if (entityIsValid)
			{
				info.entity = pEntity;
				_iterator.entities.push_back(info);
			}
		}
	}
}

void EntityComponentSystem::fillEventIteratorInternal(TypeFilter& _eventFilter, EventTypeIterator& _iterator)
{
	for (TypeID typeID : _eventFilter.requirements)
	{
		_iterator.eventTypes[typeID] = eventMgr.getEventIterator(typeID);
	}
}
