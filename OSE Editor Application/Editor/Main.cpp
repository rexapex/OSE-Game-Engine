#include "stdafx.h"
#include "Main.h"
#include "OSE-Core/Game/Game.h"
#include "OSE-Core/File System/FileSystemUtil.h"
#include "OSE-Core/Game/Camera/EditorCamera.h"
#include "OSE-Core/Input/InputSettings.h"

int main(int argc, char * argv[])
{
	using namespace ose;

	// TODO - might need to destroy resources before returning error

	// Create a game object
	auto game = ose::make_unique<Game>();

	std::string home_dir;
	fs::GetHomeDirectory(home_dir);

	//TODO - FIND DOCUMENT DIRECTORY FOR MAC & LINUX - DONE - NEEDS TESTING
	//TODO - CREATE DIRECTORIES IF THEY DON'T EXIST  - DONE - NEEDS TESTING
	std::string project_path = home_dir + "/Origami_Sheep_Engine/Projects/" + "OSE-TestProject";
	fs::CreateDirs(project_path);

	// load the project, giving access to all of its settings and scenes
	try {
		game->LoadProject(project_path);
	} catch(std::invalid_argument const & e) {
		LOG_ERROR(e.what());
		getchar();
		return 1;
	} catch(std::exception const & e) {
		LOG_ERROR(e.what());
		getchar();
		return 1;
	}

	// load a scene, giving access to all of its entities and resources
	try {
		game->LoadScene("scene1");
	} catch(std::invalid_argument const & e) {
		LOG_ERROR(e.what());
		getchar();
		return 1;
	} catch(std::exception const & e) {
		LOG_ERROR(e.what());
		getchar();
		return 1;
	}

	// set the new scene as the active scene
	try {
		game->SetActiveScene("scene1");
	} catch(std::invalid_argument const & e) {
		LOG_ERROR(e.what());
		getchar();
		return 1;
	}

	// set the active camera
	uptr<EditorCamera> camera { ose::make_unique<EditorCamera>() };
	game->SetActiveCamera(camera.get());

	// set inputs for the editor camera to use
	InputSettings settings;
	settings.axis_inputs_.emplace("move_x", AxisInput{ EInputType::A, EInputType::LEFT, EInputType::D, EInputType::RIGHT });
	settings.axis_inputs_.emplace("move_y", AxisInput{ EInputType::W, EInputType::UP, EInputType::S, EInputType::DOWN });
	settings.boolean_inputs_.emplace("LSHIFT", BooleanInput{ EInputType::LEFT_SHIFT });
	settings.boolean_inputs_.emplace("MIDDLE_MOUSE", BooleanInput{ EInputType::MOUSE_BUTTON_MIDDLE });
	game->ApplyInputSettings(settings);

	// all resources have been loaded and entities initialised, therefore, start the game
	game->StartGame();
	
	return 0;
}
