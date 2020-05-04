#pragma once

#include "ecsGlobals.h"
#include "ecsIDGenerator.h"

namespace ecs
{
	struct BaseComponent;
	typedef ID(*ComponentCreateFunction)(void* _memoryPtr, BaseComponent* _initialData);
	typedef void(*ComponentFreeFunction)(BaseComponent* _initialData);

	/*
		BaseComponent is a top level struct in the component inheritance.
		This struct handles all non-type specific information about the
		component, like entity ID and component ID.
	*/
	struct BaseComponent
	{
	private:
		ID id = 0;
		ID entityID = 0;

	public:
		bool alive = true;

		ID getID() { return id; }
		ID getEntityID() { return entityID; }

		virtual size_t getSize() { return 0; }
		virtual TypeID getTypeID() { return 0; }
		virtual std::string getName() { return ""; }
		virtual ComponentCreateFunction getCreateFunction() { return nullptr; }
		virtual ComponentFreeFunction getFreeFunction() { return nullptr; }

	protected:
		static TypeID typeIDCounter;
		static TypeID uniqueIDCounter;

	private:
		template <typename T>
		friend ID componentCreate(void* _memoryPtr, BaseComponent* _initialData);
		friend class EntityComponentSystem;
	};

	/*
		ECSComponent is the struct all user components will inherit from.
		This struct handles all type specific information, like type ID
		and create function.
	*/
	template <typename T>
	struct ECSComponent : public BaseComponent
	{
		static const size_t size;
		static const TypeID typeID;
		static const std::string name;
		static const ComponentCreateFunction createFunction;
		static const ComponentFreeFunction freeFunction;

		virtual size_t getSize() { return T::size; }
		virtual TypeID getTypeID() { return T::typeID; }
		virtual std::string getName() { return T::name; }
		virtual ComponentCreateFunction getCreateFunction() { return ECSComponent<T>::createFunction; }
		virtual ComponentFreeFunction getFreeFunction() { return ECSComponent<T>::freeFunction; }
	};

	/*
		Defines a dynamic create function. This function will use the place-
		ment operator to place the new object in already allocated memory.
	*/
	template <typename T>
	ID componentCreate(void* _memoryPtr, BaseComponent* _initialData)
	{
		//_initialData->id = BaseComponent::uniqueIDGenerator.generateID();
		_initialData->id = BaseComponent::uniqueIDCounter++;
		new(_memoryPtr) T(*(T*)_initialData);
		return _initialData->id;
	}

	/*
		Defines a dynamic free function. This function will call the correct
		destructor of the component.
	*/
	template <typename T>
	void componentFree(BaseComponent* _initialData)
	{
		T* p_typed_ptr = (T*)_initialData;
		p_typed_ptr->~T();
	}



	/*
		Initialize all static variables
	*/

	template <typename T>
	const size_t ECSComponent<T>::size(sizeof(T));

	template <typename T>
	const TypeID ECSComponent<T>::typeID(BaseComponent::typeIDCounter++);

	template <typename T>
	const std::string ECSComponent<T>::name(__nameof<T>());

	template <typename T>
	const ComponentCreateFunction ECSComponent<T>::createFunction(componentCreate<T>);

	template <typename T>
	const ComponentFreeFunction ECSComponent<T>::freeFunction(componentFree<T>);
}