#pragma once

#include "OSE-Core/Types.h"
#include "Scene/SceneManager.h"
#include "OSE-Core/Entity/EntityList.h"
#include "OSE-Core/Input/InputManager.h"
#include "ThreadManager.h"
#include "Time.h"
#include <ctime>

namespace ose
{
	class Scene;
	class Project;
	class ProjectLoader;
	class WindowManager;
	class RenderingEngine;
	class Entity;
	class Component;
	class SpriteRenderer;
	class ScriptingEngine;
	struct CustomObject;

	// Represents a runtime object of a game
	// Provides a simple way of interacting with the game
	// TODO - Use template inheritance to allow game type with different scene switch type
	class Game : public SceneManager, public EntityList, public InputManager
	{
	public:
		Game();
		virtual ~Game() noexcept;
		Game(Game &) = delete;
		Game(Game && other) noexcept = default;
		Game & operator=(Game &) = delete;
		Game & operator=(Game &&) noexcept = delete;

		// Start execution of the game
		void StartGame();

		// Activate an entity along with activated sub-entities
		// Should NEVER be called directly by a script, enable entity instead
		void OnEntityActivated(Entity & entity);

		// Deactivate an entity along with all its sub-entities
		// Should NEVER be called directly by a script, disable entity instead
		void OnEntityDeactivated(Entity & entity);

		// Get the time object
		Time const & GetTime() { return time_; }

		// Load a custom data file
		std::unique_ptr<CustomObject> LoadCustomDataFile(std::string const & path);

		// Save a custom data file
		void SaveCustomDataFile(std::string const & path, CustomObject const & object);

	protected:
		// Called upon a project being activated
		// Project is activated upon successful load
		// Only one project can be active at a time
		virtual void OnProjectActivated(Project & project);

		// Called upon a project being deactivated
		// Project is deactivated when a new project is loaded
		virtual void OnProjectDeactivated(Project & project);

		// Called upon a scene being activated
		// Depending on switch manager, could be multiple active scenes
		virtual void OnSceneActivated(Scene & scene);

		// Called upon a scene being deactivated
		// Depending on switch manager, could be multiple active scenes
		virtual void OnSceneDeactivated(Scene & scene);

	private:
		// Window manager handles window creation, events and input
		std::unique_ptr<WindowManager> window_manager_;

		// Thread manager handles multithreading and updating of engines
		///std::unique_ptr<ThreadManager> thread_manager_;

		// Rendering engine handles all rendering of entity render objects
		std::unique_ptr<RenderingEngine> rendering_engine_;

		// Scripting engine handles execution of game developer created scripts
		std::unique_ptr<ScriptingEngine> scripting_engine_;

		// TODO - current iteration of render pool
		///std::unique_ptr<RenderPool> render_pool_;

		// Time handles calculation of delta time, fps etc. and provides a way for scripts to get the timing variables
		Time time_;

		// True iff the game is currently running (paused is a subset of running)
		bool running_;

		// Called from startGame, runs a loop while running_ is true
		void RunGame();
	};
}
