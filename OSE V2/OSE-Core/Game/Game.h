#pragma once

#include "OSE-Core/Types.h"
#include "Scene/SceneSwitchManager.h"
#include "ThreadManager.h"
#include "Time.h"
#include "OSE-Core/Entity/EntityList.h"
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
	// TODO - Use template inheritance to allow game type with different scene switch type
	class Game : public SceneSwitchManager, public ose::entity::EntityList
	{
	public:
		Game();
		virtual ~Game() noexcept;
		Game(Game &) = delete;
		Game & operator=(Game &) = delete;
		Game(Game && other) noexcept = default;
		Game & operator=(Game && other) noexcept = default;

		// Called upon a scene being activated
		// Depending on switch manager, could be multiple active scenes
		virtual void OnSceneActivated(Scene & scene);

		// Start execution of the game
		void StartGame();

	private:
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
