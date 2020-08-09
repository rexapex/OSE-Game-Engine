#pragma once

#include "OSE-Core/Game/Scene/SceneManager.h""
#include "OSE-Core/Game/Camera/Camera.h"
#include "OSE-Core/Resources/Texture/Texture.h"
#include "OSE-Core/Math/Transform.h"

class SimpleApp;
class CefBrowser;

namespace ose
{
	class CefAdaptor;
	class WindowManager;
	class RenderingEngine;
	class InputManager;
	class Entity;
	class Scene;
	class Project;
	class Texture;
	class SpriteRenderer;
}

namespace ose::editor
{
	class Controller final : public SceneManager
	{
	public:
		Controller(std::unique_ptr<CefAdaptor> cef_adaptor);
		~Controller();

		// Start execution of the controller
		void StartController();

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

		// Update the editor gui texture data
		void UpdateGuiTexture(IMGDATA data, int32_t width, int32_t height);

		// Set the active camera
		// If c is nullptr, the active camera is set to the default camera
		// If the user destroys the active camera, the active camera must be set to nullptr (or a valid camera) to prevent errors
		void SetActiveCamera(unowned_ptr<Camera> c) { active_camera_ = c ? c : &default_camera_; active_camera_->SetGameReference(nullptr); }

		// Get the active camera
		unowned_ptr<Camera> GetActiveCamera() const { return active_camera_; }

		// Get the window manager
		WindowManager const & GetWindowManager() const { return *window_manager_; }

	private:
		// Window manager handles window creation, events and input
		std::unique_ptr<WindowManager> window_manager_;

		// Rendering engine handles all rendering of entity render objects
		std::unique_ptr<RenderingEngine> rendering_engine_;

		// Input manager handles mouse, keyboard, etc. input from the user
		std::unique_ptr<InputManager> input_manager_;

		// The sprite renderer component / corresponding texture / transform of the editor gui
		std::unique_ptr<SpriteRenderer> gui_renderer_;
		std::unique_ptr<Texture> gui_texture_;
		Transform gui_transform_;

		// Interface to cef controls
		std::unique_ptr<CefAdaptor> cef_adaptor_;

		// The active camera (rendering is done relative to the active camera transform)
		unowned_ptr<Camera> active_camera_ { nullptr };

		// The default camera
		Camera default_camera_;

		// True iff the controller is currently running (paused is a subset of running)
		bool running_;

		// Called from StartController, runs a loop while running_ is true
		void RunController();

		// Initialise components of an entity along with its sub-entities
		void InitEntity(Entity & entity);
	};
}
