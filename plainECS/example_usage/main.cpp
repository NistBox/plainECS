#include <iostream>

#include "ecs.h"
#include "systems.h"
#include "components.h"

#ifdef _DEBUG
#pragma comment(lib, "plainECS_Debug.lib")
#else
#pragma comment(lib, "plainECS_Release.lib")
#endif

/*
	Imagine a simple 1-dimentional world. Objects are born in the air, falls
	due to gravity and are removed from the world once they hit the ground.
*/

int main()
{
	ecs::EntityComponentSystem world;

	/*
		-- CREATE SYSTEMS --

		Create the systems for the world. Order matters.

		GravitySystem		-		Updates the position of each objects that
									has a GravityComponent and a PositionComponent.

		GroundHitSystem		-		Checks the position of each object with a
									PositionComponent, removes those who has hit
									the ground.

		These systems are found in 'systems.h' and 'systems.cpp'
	*/

	world.createSystem<ecs::systems::GravitySystem>();
	world.createSystem<ecs::systems::GroundHitSystem>();



	/*
		-- CREATE ENTITIES --

		PhysicsComponent	-		Holds information about its object's mass and
									velocity.

		PositionComponent	-		Holds information about its object's height
									in the world.								

		These components are found in 'components.h'
	*/

	PhysicsComponent physics_desc;
	physics_desc.mass = 1.0f;
	physics_desc.velocity = 0.0f;

	PositionComponent position_desc;

	// Create 10 entitites
	for (int i = 1; i <= 10; i++)
	{
		position_desc.height = float(i);
		world.createEntity(physics_desc, position_desc);
	}


	/*
		-- MAIN LOOP --

		Simulate a world with 60 ticks (updates) per second
	*/

	const float delta = 1.f / 60.f;
	float elapsed = 0.f;

	while (world.getTotalEntityCount() > 0)
	{
		world.update(delta);

		elapsed += delta;
		std::cout << "Elapsed time: " << elapsed << "\nEntity count: " << world.getTotalEntityCount() << "\n\n";
	}

	system("pause");

	return 0;
}