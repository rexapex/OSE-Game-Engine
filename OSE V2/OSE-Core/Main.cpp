#include "stdafx.h"
#include "Main.h"
///#include "Math/Transform.h"

int main(int argc, char * argv[])
{
	using namespace ose;

	// process each command-line argument
	for(int i = 0; i < argc; i++)
	{
		std::string arg = argv[i];

		// edit arg means the engine should be opened in editor mode
		if(arg == "edit") {
			
		}
	}

	///using namespace ose::math;
	///Transform t { { 0, 832043982, -432098 }, { 0, 0, 0 }, { 1, 1, 1 } };
	///t.rotate(3.14159265359, 0, 0);
	///t.rotateDeg({180, 0, 0});
	///DEBUG_LOG(t.get_up().x << ", " << t.get_up().y << ", " << t.get_up().z);
	///DEBUG_LOG(t.get_forward().x << ", " << t.get_forward().y << ", " << t.get_forward().z);
	///DEBUG_LOG(t.get_right().x << ", " << t.get_right().y << ", " << t.get_right().z);

	/// std::unique_ptr<Game> game = std::make_unique<Game>();
	
	// TODO - might need to destroy resources before returning error
	
	// load the project, giving access to all of its settings and scenes
	try {
		Env::game->LoadProject("OSE-TestProject");
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
		Env::game->LoadScene("scene1");
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
		Env::game->SetActiveScene("scene1");
	} catch(const std::invalid_argument & e) {
		ERROR_LOG(e.what());
		getchar();
		return 1;
	}

	// all resources have been loaded and entities initialised, therefore, start the game
	Env::game->StartGame();
	
	return 0;
}
