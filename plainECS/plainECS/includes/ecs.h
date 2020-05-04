#pragma once

#include <map>
#include <unordered_map>
#include <vector>
#include "ecsComponent.h"
#include "ecsSystem.h"
#include "ecsGlobals.h"
#include "ecsComponentManager.h"
#include "ecsEntityManager.h"
#include "ecsEventManager.h"
#include "ecsUser.h"
#include "ecsEventListener.h"
#include "ecsEventIterator.h"
#include "ecsTypeFilter.h"
#include "ecsEvent.h"

#define DEFAULT_LAYER_COUNT 10

//#ifdef _DEBUG
//#define DEBUG_ENTITY_PRINT_MAX_COUNT 20
//#endif

// TODO: Move event creations for new entity and component out of internal functions.

namespace ecs
{
	struct CompTypeMemDesc
	{	
		TypeID typeID;
		size_t componentSize;
		size_t memoryCount;
	};

	struct ECSDesc
	{
		CompTypeMemDesc* compTypeMemDescs;
		size_t compTypeCount;
		size_t systemLayerCount;
	};

	// EntityComponentSystem is the main class and API for using plainECS. It stores all ECSSystems
	// and updates them appropriately during an ECS update. This class has multiple managers, and
	// handles all communication between them. These managers are
	//		- ECSEntityManager
	//		- ECSComponentManager
	//		- ECSEventManager
	// Most API functions forwards requests for these managers.
	class EntityComponentSystem : public ECSUserListener, public ECSEventListenerListener
	{
	public:
		EntityComponentSystem();
		~EntityComponentSystem();

		// Initializes ECS for custom uses, using user set specifications on memory from an ECSDesc(ription). If this function is not
		// called, the ECS will set memory usage to default values:
		//		- Number of system layers is set to 10
		//		- All component pools will have a size of 100 components (of that type). The pools will happen when a component of an
		//		 uninitialized component pool is encountered during runtime.
		//			- This will also happen if a component type is not specified in the ECSDesc.
		bool initialize(ECSDesc &_desc);

		template <typename T>
		void reserveComponentCount(unsigned int _count);

		// Creates an entity with given components. Components are created in component memory using the initial information from input.
		// Will create a CreateEntityEvent and a CreateComponentEvent automatically.
		//		Notation: the input component will only be used to set initial values of the new component, thus changing
		//				 the input component after createEntity does not change the new component in ECS memory.
		Entity* createEntity(BaseComponent& _comp);

		// Creates an entity with given components. Components are created in component memory using the initial information from input.
		// Will create a CreateEntityEvent and two CreateComponentEvents automatically.
		//		Notation: the input components will only be used to set initial values of the new components, thus changing
		//				 the input component after createEntity does not change the new components in ECS memory.
		Entity* createEntity(BaseComponent& _compA, BaseComponent& _compB);

		// Creates an entity with given components. Components are created in component memory using the initial information from input.
		// Will create a CreateEntityEvent and three CreateComponentEvents automatically.
		//		Notation: the input components will only be used to set initial values of the new components, thus changing
		//				 the input components after createEntity does not change the new components in ECS memory.
		Entity* createEntity(BaseComponent& _compA, BaseComponent& _compB, BaseComponent& _compC);

		// Creates an entity with given components. Components are created in component memory using the initial information from input.
		// Will create a CreateEntityEvent and four CreateComponentEvents automatically.
		//		Notation: the input components will only be used to set initial values of the new components, thus changing
		//				 the input components after createEntity does not change the new components in ECS memory.
		Entity* createEntity(BaseComponent& _compA, BaseComponent& _compB, BaseComponent& _compC, BaseComponent& _compD);

		// Creates an entity with given components. Components are created in component memory using the initial information from input.
		// Will create a CreateEntityEvent and five CreateComponentEvents automatically.
		//		Notation: the input components will only be used to set initial values of the new components, thus changing
		//				 the input components after createEntity does not change the new components in ECS memory.
		Entity* createEntity(BaseComponent& _compA, BaseComponent& _compB, BaseComponent& _compC, BaseComponent& _compD, BaseComponent& _compE);

		// Creates an entity from a list of components. Components are created in component memory.
		// Will create a CreateEntityEvent and multiple CreateComponentEvents automatically.
		//		Notation: the input components will only be used to set initial values of the new components, thus changing
		//				 the input components after createEntity does not change the new components in ECS memory.
		Entity* createEntity(ComponentList _components);

		// First removes all components a given entity has, then removes the entity itself.
		// Will create a RemoveEntityEvent automatically.
		void removeEntity(ID _entityID);

		// Creates a component in component memory and updates its entity with its ID.
		// Will create a CreateComponentEvent.
		template <typename T> T* createComponent(ID _entityID, T& _component);

		// Removes component from memory and updates its entity about the removal.
		// Will create a RemoveComponentEvent.
		void removeComponent(ID _entityID, TypeID _componentTypeID);

		// Creates a system and pushes it back in wanted priority layer for later updates.
		// The system is responsible for inializing itself with UpdateTypes and component/event
		// filter in its constructor and for overriding the correct update function.
		template <typename T> T* createSystem(unsigned int layer = 1);

		// Finds system of given type and removes in from memory, using its destructor.
		template <typename T> void removeSystem();

		// Finds system of given type and returns a pointer to it.
		template <typename T> T* getSystem();

		// Creates an event in the ECS.
		// This will notify all event listeners that has subscribed to that event type.
		void createEvent(BaseEvent& _event);

		// Initiate an ECS update, iterating through all layers and updates all system based
		// on their SystemUpdateType.
		void update(float _delta);

		/*
		*	Getters
		*/

		// Returns a pointer to the entity with given ID.
		Entity* getEntity(ID _id);

		// Returns a base pointer to the component of given TypeID and ID.
		BaseComponent* getComponent(TypeID _typeID, ID _id);

		// Returns a base pointer to the component of given TypeID that belongs given entity.
		BaseComponent* getComponentFromEntity(TypeID _typeID, ID _entityID);
		
		// Returns a casted pointer to the component of given type and ID.
		template <typename T> T* getComponent(ID _id);

		// Returns a casted pointer to the component of given type that belongs to given entity.
		template <typename T> T* getComponentFromEntity(ID _entityID);

		// Returns the number of layers that exists in the ECS.
		size_t getSystemLayerCount();

		// Returns the total number of systems in the ECS.
		size_t getTotalSystemCount();

		// Returns the total number of entities in the ECS.
		size_t getTotalEntityCount();

		// Returns the total number of components in the ECS.
		size_t getTotalComponentCount();

		// Returns the number of initialized component types in the ECS.
		// Uninitialized component types don't yet have a component pool.
		size_t getComponentTypeCount();

		// Returns the number of components of given type in the ECS.
		size_t getComponentCountOfType(TypeID _typeID);
		
		// Returns an EntityIterator that only contains entities that have all components
		// in given component type filter.
		EntityIterator getEntititesByFilter(TypeFilter _componentFilter);

		// Returns a ComponentIterator that only contains components of given TypeID.
		ComponentIterator getAllComponentsOfType(TypeID _typeID);

		// Returns a TypeFilter that contain the TypeID of all initialized component types.
		// An initialized component type has an existing component pool.
		TypeFilter getInitializedComponentTypes();

	private:

		ECSEventManager eventMgr;
		ECSEntityManager entityMgr;
		ECSComponentManager componentMgr;

		using SystemList = std::vector<BaseSystem*>;
		size_t layerCount;

		// Dynamic list of all system layers.
		SystemList *systemLayers;

		// Map containing one entry per system in the ECS.
		// Can be used to find which layer a system is in for quick access.
		// As each system has an entry in this map, its size equals the number of systems.
		std::map<TypeID, unsigned int> typeIDToLayerMap;

		/*
		*	ECSUserListener overriden functions
		*/

		Entity* onGetEntity(ID _entityID) override;
		BaseComponent* onGetComponent(TypeID _typeID, ID _id) override;
		Entity* onCreateEntity(ComponentList _components) override;
		BaseComponent* onCreateComponent(ID _entityID, BaseComponent& _componentInfo) override;
		EntityIterator onGetEntitiesByFilter(TypeFilter _componentFilter) override;
		ComponentIterator onGetComponentsOfType(TypeID _typeID) override;
		void onCreateEvent(BaseEvent& _event) override;
		void onRemoveEntity(ID _entityID) override;
		void onRemoveComponent(ID _entityID, TypeID _componentTypeID) override;

		void* onGetSystem(TypeID _typeID) override;
		void* onCreateSystem(void* _tempSystem, unsigned int _layer) override;
		void onRemoveSystem(TypeID _typeID) override;

		int onGetComponentCountOfType(TypeID _typeID) override;

		/*
		*	ECSEventListenerListener (yes, double Listener) overriden functions
		*/

		virtual void onAddSubscription(TypeID _eventTypeID, ECSEventListener* _listener);
		virtual void onRemoveSubscription(TypeID _eventTypeID, ECSEventListener* _listener);

		/*
		*	Internal functionality
		*/

		// Creates a new entity in the ECS, then creates all its components.
		Entity* createEntityInternal(ComponentList _components);

		BaseComponent* createComponentInternal(ID _entityID, BaseComponent& _componentInfo);
		void createEventInternal(BaseEvent& _event);
		void removeEntityInternal(ID _entityID);
		void removeComponentInternal(ID _entityID, TypeID _componentTypeID);
		void fillEntityIteratorInternal(TypeFilter& _componentFilter, EntityIterator& _iterator);
		void fillEventIteratorInternal(TypeFilter& _eventFilter, EventTypeIterator& _iterator);

		//std::unordered_map<TypeID, > systemCreateFunctions;
	};

	/*
	*	Templated functions (has to be in header)
	*/

	template <typename T>
	void EntityComponentSystem::reserveComponentCount(unsigned int _count)
	{
		componentMgr.initPoolType(T::typeID, T::size, _count);
	}

	template<typename T>
	T* EntityComponentSystem::createComponent(ID _entityID, T& _component)
	{
		// Forwards to internal function and cast returning BaseComponent pointer
		return (T*)createComponentInternal(_entityID, _component);
	}

	template<typename T>
	inline T* EntityComponentSystem::createSystem(unsigned int layer)
	{
		// Check if systemLayers been initialized
		if (!systemLayers)
		{
			layerCount = 10;
			systemLayers = new SystemList[10];
		}

		// Check if system is already added,
		// should only exist one system at most per type
		if (typeIDToLayerMap.count(T::typeID) != 0)
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
		T* newSystem = new T;

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
		typeIDToLayerMap[T::typeID] = layer;

		// Push back system in wanted layer for later update
		systemLayers[layer].push_back(newSystem);
		return newSystem;
	}

	template<typename T>
	inline void EntityComponentSystem::removeSystem()
	{
		// Check if system exist
		if (typeIDToLayerMap.count(T::typeID) == 0)
		{
			return;
		}

		// Retrieve the system's layer
		SystemList& layer = systemLayers[typeIDToLayerMap[T::typeID]];

		// Iterate in layer until system is found
		for (size_t i = 0; i < layer.size(); i++)
		{
			// Check if wanted system
			if (layer[i]->getTypeID() == T::typeID)
			{
				BaseSystem* sys = layer[i];						// Store system pointer
				layer.erase(layer.begin() + i);					// Remove system from layer
				//SystemFreeFunction ff = sys->getFreeFunction(); /* Fetch free function, in case user hasn't
				//												   written its destructor as virtual */
				//ff(sys);										// Call free function
				delete sys;
				typeIDToLayerMap.erase(T::typeID);				// Erase from hash map
				return;
			}
		}
	}

	template<typename T>
	inline T* EntityComponentSystem::getSystem()
	{
		return (T*)onGetSystem(T::typeID);
	}

	inline void EntityComponentSystem::createEvent(BaseEvent& _event)
	{
		return createEventInternal(_event);
	}

	template <typename T> inline T* EntityComponentSystem::getComponent(ID _id)
	{
		// Forwards to internal function and cast returning BaseComponent pointer
		return (T*)getComponent(T::typeID, _id);
	}

	template <typename T> inline T* EntityComponentSystem::getComponentFromEntity(ID _entityID)
	{
		// Retrieve entity and then fetch the ID of wanted component type
		Entity* e = getEntity(_entityID);

		// Sanity check
		if (!e->hasComponentOfType<T>())
		{
			return nullptr;
		}

		return getComponent<T>(e->getComponentID<T>());
	}
}