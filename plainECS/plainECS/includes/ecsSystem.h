#pragma once

#include <iostream>
#include "ecsBaseSystem.h"
#include "ecsComponent.h"
#include "ecsEvent.h"
#include "ecsTypeFilter.h"

using namespace std;
using namespace ecs::components;
using namespace ecs::events;

namespace ecs
{
	namespace systems
	{
		// Leave ecs::systems namespaces, so that plainECS can compile with it.
	}
}