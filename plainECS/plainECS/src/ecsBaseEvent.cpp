#include "ecsBaseEvent.h"

using namespace ecs;


// Make id counters start at 1, since 0=error
TypeID BaseEvent::typeIDCounter(1);
//IDGenerator<TypeID> BaseEvent::typeIDGenerator(1);
