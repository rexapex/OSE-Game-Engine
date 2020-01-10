#include "stdafx.h"
#include "Main.h"
#include "OSE-Core/Game/Game.h"

int main(int argc, char * argv[])
{
	using namespace ose;

	// TODO - might need to destroy resources before returning error

	// Create a game object
	auto game = std::make_unique<Game>();

	// load the project, giving access to all of its settings and scenes
	try {
		game->LoadProject("OSE-TestProject");
	} catch(const std::invalid_argument & e) {
		ERROR_LOG(e.what());
		return 1;
	} catch(const std::exception & e) {
		ERROR_LOG(e.what());
		getchar();
		return 1;
	}

	// load a scene, giving access to all of its entities and resources
	try {
		game->LoadScene("scene1");
	} catch(const std::invalid_argument & e) {
		ERROR_LOG(e.what());
		return 1;
	} catch(const std::exception & e) {
		ERROR_LOG(e.what());
		getchar();
		return 1;
	}

	// set the new scene as the active scene
	try {
		game->SetActiveScene("scene1");
	} catch(const std::invalid_argument & e) {
		ERROR_LOG(e.what());
		getchar();
		return 1;
	}

	// all resources have been loaded and entities initialised, therefore, start the game
	game->StartGame();
	
	return 0;
}
