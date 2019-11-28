#pragma once

#include "OSE-Core/Types.h"
#include "ESceneSwitchMode.h"
#include "ThreadManager.h"
#include "Time.h"
#include <ctime>

namespace ose
{
	namespace project
	{
		class Project;
		class ProjectLoader;
	}
	namespace windowing
	{
		class WindowManager;
	}
	namespace rendering
	{
		class RenderingEngine;
	}
	namespace entity
	{
		class Entity;
		class Component;
		class SpriteRenderer;
	}
}

namespace ose::game
{
	class Scene;

	// Represents a runtime object of a game
	// Provides a simple way of interacting with the game
	class Game
	{
	public:
		Game();
		~Game() noexcept;
		Game(Game &) = delete;
		Game & operator=(Game &) = delete;
		Game(Game && other) noexcept = default;
		Game & operator=(Game && other) noexcept = default;
		
		// Scene methods
		void LoadProject(const std::string & proj_name);		// Loads the project specified (does not load any scenes)
		void LoadScene(const std::string & scene_name);			// Loads the scene into memory
		void UnloadScene(const std::string & scene_name);		// Frees the scene from memory
		void UnloadAllLoadedScenes();							// Frees all loaded scenes (not active scene) from memory
		void SetActiveScene(const std::string & scene_name);	// Switches game to the scene specified iff it is loaded

		// Provide const and non-const versions
		///EntityList & persistent_entities() { return persistent_entities_; };
		///const EntityList & persistent_entities() const { return persistent_entities_; }

		// Set the way the game removes scenes on a scene switch
		void SetSceneSwitchMode(const ESceneSwitchMode & mode) {scene_switch_mode_ = mode;}

		// Start execution of the game
		void StartGame();

	private:
		// Specifies which scenes should be unloaded on scene switch
		ESceneSwitchMode scene_switch_mode_;

		std::unique_ptr<ose::project::Project> project_;
		std::unique_ptr<ose::project::ProjectLoader> project_loader_;

		// The current scene being played (updated, rendered, etc...)
		std::unique_ptr<Scene> active_scene_;

		// Contains every scene which has been loaded but NOT the active scene
		std::map<std::string, std::unique_ptr<Scene>> loaded_scenes_;

		// Entities which will persist between scenes
		///EntityList persistent_entities_;

		// Window manager handles window creation, events and input
		std::unique_ptr<ose::windowing::WindowManager> window_manager_;

		// Thread manager handles multithreading and updating of engines
		///std::unique_ptr<ThreadManager> thread_manager_;

		// Rendering engine handles all rendering of entity render objects
		std::unique_ptr<ose::rendering::RenderingEngine> rendering_engine_;

		// TODO - current iteration of render pool
		///std::unique_ptr<RenderPool> render_pool_;

		// Time handles calculation of delta time, fps etc. and provides a way for scripts to get the timing variables
		Time time_;

		// True iff the game is currently running (paused is a subset of running)
		bool running_;

		// Called from startGame, runs a loop while running_ is true
		void RunGame();

		// Initialise components of an entity along with its sub-entities
		void InitEntity(const ose::entity::Entity & entity);
	};
}
