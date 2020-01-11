#pragma once

#include "OSE-Core/Game/Scene/SceneSwitchManager.h"

namespace ose
{
	class WindowManager;
	class RenderingEngine;
	class InputManager;
	class Entity;
	class Scene;
	class Project;
	class Texture;
}

namespace ose::editor
{
	class Controller final : public SceneSwitchManager
	{
	public:
		Controller();
		~Controller();

		// Called upon a project being activated
		// Project is activated upon successful load
		// Only one project can be active at a time
		virtual void OnProjectActivated(Project & project);

		// Called upon a scene being activated
		// Depending on switch manager, could be multiple active scenes
		virtual void OnSceneActivated(Scene & scene);

		// Render the active scene
		void Render(Texture & gui);

		// Get the window manager
		WindowManager const & GetWindowManager() const { return *window_manager_; }

	private:
		// Window manager handles window creation, events and input
		std::unique_ptr<WindowManager> window_manager_;

		// Rendering engine handles all rendering of entity render objects
		std::unique_ptr<RenderingEngine> rendering_engine_;

		// Input manager handles mouse, keyboard, etc. input from the user
		std::unique_ptr<InputManager> input_manager_;

		// Initialise components of an entity along with its sub-entities
		void InitEntity(Entity & entity);
	};
}
