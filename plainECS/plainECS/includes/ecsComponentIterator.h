#pragma once

#include "ecsBaseComponent.h"

/*
*	ComponentIterator is the interface for iteration on component memory.
*	It handles reading unnecessary memory, like removed component junk.
*	In order to use it, you have to request it since it can only be created
*	properly through a ComponentPool.
*/
namespace ecs
{
	class ComponentIterator
	{
	public:
		ComponentIterator();
		ComponentIterator(size_t _objectSize, size_t _totalSize, void* _memory);
		~ComponentIterator();

		BaseComponent* next();

	private:
		void* first;
		void* current;
		size_t totalSize;
		size_t objectSize;
		size_t iterationSize;
	};
}