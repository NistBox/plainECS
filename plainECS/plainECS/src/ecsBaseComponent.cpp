#include "ecsBaseComponent.h"

using namespace ecs;

// Make id counters start at 1, since 0=error
TypeID BaseComponent::typeIDCounter(1);
TypeID BaseComponent::uniqueIDCounter(1);
