#include "stdafx.h"
#include "Main.h"
#include "OSE-Core/Game/Game.h"
#include "OSE-Core/File System/FileSystemUtil.h"
#include "OSE-Core/Game/Camera/EditorCamera.h"
#include "OSE-Core/Input/InputSettings.h"

#include "include/cef_sandbox_win.h"
#include "Cef/CefApp.h"

int main(int argc, char * argv[])
{
	using namespace ose;

	// Enable High-DPI support on Windows 7 or newer.
	CefEnableHighDPISupport();

	void* sandbox_info = NULL;

#if defined(CEF_USE_SANDBOX)
	// Manage the life span of the sandbox information object. This is necessary
	// for sandbox support on Windows. See cef_sandbox_win.h for complete details.
	CefScopedSandboxInfo scoped_sandbox;
	sandbox_info = scoped_sandbox.sandbox_info();
#endif

	// Provide CEF with command-line arguments.
#ifdef _WIN32
	CefMainArgs main_args(NULL);
#else
	CefMainArgs main_args(argc, argv);
#endif

	// CEF applications have multiple sub-processes (render, plugin, GPU, etc)
	// that share the same executable. This function checks the command-line and,
	// if this is a sub-process, executes the appropriate logic.
	int exit_code = CefExecuteProcess(main_args, NULL, sandbox_info);
	if (exit_code >= 0)	// The sub-process has completed so return here.
		return exit_code;

	// Specify CEF global settings here.
	CefSettings settings;

#if !defined(CEF_USE_SANDBOX)
	settings.no_sandbox = true;
#endif

	// SimpleApp implements application-level callbacks for the browser process.
	// It will create the first browser instance in OnContextInitialized() after
	// CEF has initialized.
	CefRefPtr<SimpleApp> app(new SimpleApp);

	// Initialize CEF.
	CefInitialize(main_args, settings, app.get(), sandbox_info);

	// Run the CEF message loop. This will block until CefQuitMessageLoop() is
	// called.
	CefRunMessageLoop();

	// Shut down CEF.
	CefShutdown();

	std::string home_dir;
	fs::GetHomeDirectory(home_dir);

	//TODO - FIND DOCUMENT DIRECTORY FOR MAC & LINUX - DONE - NEEDS TESTING
	//TODO - CREATE DIRECTORIES IF THEY DON'T EXIST  - DONE - NEEDS TESTING
	std::string project_path = home_dir + "/Origami_Sheep_Engine/Projects/" + "OSE-TestProject";
	fs::CreateDirs(project_path);

	// Create a game object
	auto game = std::make_unique<Game>();

	// load the project, giving access to all of its settings and scenes
	try {
		game->LoadProject(project_path);
	} catch(const std::invalid_argument & e) {
		LOG_ERROR(e.what());
		getchar();
		return 1;
	} catch(const std::exception & e) {
		LOG_ERROR(e.what());
		getchar();
		return 1;
	}

	// load a scene, giving access to all of its entities and resources
	try {
		game->LoadScene("scene1");
	} catch(const std::invalid_argument & e) {
		LOG_ERROR(e.what());
		getchar();
		return 1;
	} catch(const std::exception & e) {
		LOG_ERROR(e.what());
		getchar();
		return 1;
	}

	// set the new scene as the active scene
	try {
		game->SetActiveScene("scene1");
	} catch(const std::invalid_argument & e) {
		LOG_ERROR(e.what());
		getchar();
		return 1;
	}

	// set the active camera
	std::unique_ptr<EditorCamera> camera { std::make_unique<EditorCamera>() };
	game->SetActiveCamera(camera.get());

	// set inputs for the editor camera to use
	InputSettings input_settings;
	input_settings.axis_inputs_.emplace("move_x", AxisInput{ EInputType::A, EInputType::LEFT, EInputType::D, EInputType::RIGHT });
	input_settings.axis_inputs_.emplace("move_y", AxisInput{ EInputType::W, EInputType::UP, EInputType::S, EInputType::DOWN });
	input_settings.boolean_inputs_.emplace("LSHIFT", BooleanInput{ EInputType::LEFT_SHIFT });
	input_settings.boolean_inputs_.emplace("MIDDLE_MOUSE", BooleanInput{ EInputType::MOUSE_BUTTON_MIDDLE });
	game->ApplyInputSettings(input_settings);

	// all resources have been loaded and entities initialised, therefore, start the game
	game->StartGame();
	
	return 0;
}
