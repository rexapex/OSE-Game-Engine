#pragma once

#include "OSE-Core/EngineReferences.h"
#include "OSE-Core/Project/Project.h"
#include "OSE-Core/Project/ProjectLoader.h"
#include "OSE-Core/Windowing/WindowManager.h"
#include "OSE-Core/Rendering/RenderingEngine.h"
#include "OSE-Core/Entity/Entity.h"
#include "Scene.h"
#include "ESceneSwitchMode.h"
#include "ThreadManager.h"
#include "Time.h"
#include <ctime>

namespace ose::game
{
	using namespace entity;
	using namespace project;
	using namespace rendering;
	using namespace windowing;

	// Represents a runtime object of a game
	// Provides a simple way of interacting with the game
	class Game
	{
	public:
		Game();
		~Game() noexcept;
		Game(Game &) = delete;
		Game & operator=(Game &) = delete;
		Game(Game && other) noexcept;
		Game & operator=(Game && other) noexcept;
		
		// scene methods
		void loadProject(const std::string & proj_name);		// loads the project specified (does not load any scenes)
		void loadScene(const std::string & scene_name);			// loads the scene into memory
		void unloadScene(const std::string & scene_name);		// frees the scene from memory
		void unloadAllLoadedScenes();							// frees all loaded scenes (not active scene) from memory
		void setActiveScene(const std::string & scene_name);	// switches game to the scene specified iff it is loaded

		// provide const and non-const versions
		EntityList & persistent_entities() { return persistent_entities_; };
		const EntityList & persistent_entities() const { return persistent_entities_; }

		// set the way the game removes scenes on a scene switch
		void setSceneSwitchMode(const ESceneSwitchMode & mode) {scene_switch_mode_ = mode;}

		// start execution of the game
		void startGame();

	private:
		// specifies which scenes should be unloaded on scene switch
		ESceneSwitchMode scene_switch_mode_;

		std::unique_ptr<Project> project_;
		std::unique_ptr<ProjectLoader> project_loader_;

		// the current scene being played (updated, rendered, etc...)
		std::unique_ptr<Scene> active_scene_;

		// contains every scene which has been loaded but NOT the active scene
		std::map<std::string, std::unique_ptr<Scene>> loaded_scenes_;

		// entities which will persist between scenes
		EntityList persistent_entities_;

		// window manager handles window creation, events and input
		std::unique_ptr<WindowManager> window_manager_;

		// thread manager handles multithreading and updating of engines
		std::unique_ptr<ThreadManager> thread_manager_;

		// rendering engine handles all rendering of entity render objects
		std::unique_ptr<RenderingEngine> rendering_engine_;

		// time handles calculation of delta time, fps etc. and provides a way for scripts to get the timing variables
		Time time_;

		// true iff the game is currently running (paused is a subset of running)
		bool running_;

		// called from startGame, runs a loop while running_ is true
		void runGame();
	};
}
