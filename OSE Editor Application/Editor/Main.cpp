#include "stdafx.h"
#include "Main.h"
#include "OSE-Core/Game/Game.h"
#include "OSE-Core/Game/Scene/Scene.h"
#include "OSE-Core/File System/FileSystemUtil.h"
#include "OSE-Core/Game/Camera/EditorCamera2D.h"
#include "OSE-Core/Input/InputSettings.h"
#include "OSE-Core/Game/Scene/Chunk/ChunkManagerSettings.h"

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

	// Set inputs for the editor camera to use
	InputSettings settings;
	settings.axis_inputs_.emplace("OSE-Camera-Move-X", AxisInput{ EInputType::A, EInputType::LEFT, EInputType::D, EInputType::RIGHT });
	settings.axis_inputs_.emplace("OSE-Camera-Move-Y", AxisInput{ EInputType::W, EInputType::UP, EInputType::S, EInputType::DOWN });
	settings.boolean_inputs_.emplace("OSE-Camera-Move-Fast", BooleanInput{ EInputType::LEFT_SHIFT });
	game->ApplyInputSettings(settings);

	// Add a stub entity to follow using the camera
	EditorCamera2D camera;
	game->SetActiveCamera(&camera);

	// Override the chunk manager settings, have to reset after each scene switch
	ChunkManagerSettings chunk_settings;
	chunk_settings.agent_name_ = "";
	chunk_settings.load_distance_ = 500;
	chunk_settings.unload_distance_ = 600;
	game->GetActiveScene()->ApplyChunkManagerSettings(chunk_settings);
	game->GetActiveScene()->ResetChunkManagerAgent(game.get(), camera.GetStubEntity());

	// All resources have been loaded and entities initialised, therefore, start the game
	game->StartGame();
	
	return 0;
}
