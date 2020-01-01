#include "stdafx.h"
#include "Main.h"
#include "OSE-Core/Env.h"
#include "OSE-Core/Game/Game.h"

#include "OSE-Core/Resources/Custom Data/CustomObject.h"

int main(int argc, char * argv[])
{
	using namespace ose;

	// TODO - might need to destroy resources before returning error

	auto obj = Env::GetGame().LoadCustomDataFile(R"(D:\James\Documents\Origami_Sheep_Engine\Projects\OSE-TestProject\Resources\custom-data.xml)");
	Env::GetGame().SaveCustomDataFile(R"(D:\James\Documents\Origami_Sheep_Engine\Projects\OSE-TestProject\Resources\custom-data.xml)", *obj);

	// load the project, giving access to all of its settings and scenes
	try {
		Env::GetGame().LoadProject("OSE-TestProject");
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
		Env::GetGame().LoadScene("scene1");
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
		Env::GetGame().SetActiveScene("scene1");
	} catch(const std::invalid_argument & e) {
		ERROR_LOG(e.what());
		getchar();
		return 1;
	}

	// all resources have been loaded and entities initialised, therefore, start the game
	Env::GetGame().StartGame();
	
	return 0;
}
