#include "ecsBaseSystem.h"

using namespace ecs;

// Make id counters start at 1, since 0=error
TypeID BaseSystem::typeIDCounter(1);
//IDGenerator<TypeID> BaseSystem::typeIDGenerator(1);



//std::unordered_map<TypeID, SystemCreateFunction>* BaseSystem::typeIDToCreateFunctionMap = new std::unordered_map<TypeID, SystemCreateFunction>;
//std::unordered_map<TypeID, SystemCreateFunction> BaseSystem::typeIDToCreateFunctionMap { { 0, ecs::systemCreate<int>() } };

//SystemCreateFunction BaseSystem::RegisterCreateFunction(TypeID _typeID, SystemCreateFunction cf)
//{
//	if (BaseSystem::typeIDToCreateFunctionMap->count(_typeID))
//	{
//		int a = 0;
//	}
//	else
//	{
//		int b = 0;
//	}
//
//	//BaseSystem::typeIDToCreateFunctionMap[_typeID] = cf;
//	return cf;
//}