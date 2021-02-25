#include <ctime>
#include <iostream>

#include <Core/Engine.h>
#include <bowAndArrow/BowAndArrow.h>

int main(int argc, char *argv[]) {
	srand((unsigned int)time(NULL));

	// Create a window property structure
	WindowProperties wp;
	wp.resolution = glm::ivec2(1280, 720);

	// Init the Engine and create a new window with the defined properties
	WindowObject *window = Engine::Init(wp);

	// Create a new 3D world and start running it
	World *world = new BowAndArrow();
	world->Init();
	world->Run();
	// Signals to the Engine to release the OpenGL context
	Engine::Exit();

	// Note to self: press f12 on something that you want to see it's location
	delete world;
	return 0;
}