#include "stdafx.h"
#include "Controller.h"

#include "OSE-Core/Project/Project.h"
#include "OSE-Core/Game/Scene/Scene.h"

#include "OSE-Core/Entity/Entity.h"
#include "OSE-Core/Entity/Component/Component.h"
#include "OSE-Core/Entity/Component/SpriteRenderer.h"
#include "OSE-Core/Entity/Component/TileRenderer.h"

#include "OSE-Core/Rendering/RenderingEngine.h"
#include "OSE-Core/Windowing/WindowManager.h"
#include "OSE-Core/Input/InputManager.h"

#include "OSE-Core/EngineReferences.h"
#include "OSE-Core/Windowing/WindowingFactory.h"
#include "OSE-Core/Rendering/RenderingFactory.h"

#include "Editor/Cef/CefAdaptor.h"

namespace ose::editor
{
	Controller::Controller(std::unique_ptr<CefAdaptor> cef_adaptor) : cef_adaptor_(std::move(cef_adaptor))
	{
		this->window_manager_ = WindowingFactories[0]->NewWindowManager();
		this->window_manager_->NewWindow(1);
		int fbwidth { this->window_manager_->GetFramebufferWidth() };
		int fbheight { this->window_manager_->GetFramebufferHeight() };

		this->input_manager_ = std::make_unique<InputManager>();

		this->rendering_engine_ = std::move(RenderingFactories[0]->NewRenderingEngine(fbwidth, fbheight));
		this->window_manager_->SetEngineReferences(this);
		this->rendering_engine_->SetProjectionModeAndFbSize(EProjectionMode::ORTHOGRAPHIC, fbwidth, fbheight);

		this->gui_texture_ = RenderingFactories[0]->NewTexture("", "");
		this->gui_renderer_ = std::make_unique<SpriteRenderer>("GUI", gui_texture_.get());
		this->rendering_engine_->GetRenderPool().AddSpriteRenderer(gui_transform_, gui_renderer_.get());
		this->gui_transform_.SetTranslation(0, 0, 1);

		this->active_camera_ = &default_camera_;
	}

	Controller::~Controller()
	{
		
	}

	// Called upon a project being activated
	// Project is activated upon successful load
	// Only one project can be active at a time
	void Controller::OnProjectActivated(Project & project)
	{
		// Clear the input manager of inputs from previous projects then apply the default project inputs
		input_manager_->ClearInputs();
		input_manager_->ApplyInputSettings(project.GetInputSettings());
	}

	// Called upon a project being deactivated
	// Project is deactivated when a new project is loaded
	void Controller::OnProjectDeactivated(Project & project)
	{
		// TODO
	}

	// Called upon a scene being activated
	// Depending on switch manager, could be multiple active scenes
	void Controller::OnSceneActivated(Scene & scene)
	{
		// IMPORTANT - the following code can only be run on the same thread as the render context

		// create GPU memory for the new resources
		project_->CreateGpuResources();

		// create GPU memory for the new render objects
		for(auto const & entity : scene.GetEntities())
		{
			InitEntity(*entity);
		}
	}

	// Called upon a scene being deactivated
	// Depending on switch manager, could be multiple active scenes
	void Controller::OnSceneDeactivated(Scene & scene)
	{
		// TODO
	}

	// Update the editor gui texture data
	void Controller::UpdateGuiTexture(IMGDATA data, int32_t width, int32_t height)
	{
		gui_texture_->SetImgData(data, width, height, 4);
		gui_texture_->CreateTexture();
		rendering_engine_->GetRenderPool().RemoveSpriteRenderer(gui_renderer_.get());
		rendering_engine_->GetRenderPool().AddSpriteRenderer(gui_transform_, gui_renderer_.get());
	}

	void Controller::StartController()
	{
		if(!running_)
		{
			running_ = true;
			RunController();
		}
		else
		{
			LOG("Error: cannot start controller, controller is already running");
		}
	}

	void Controller::RunController()
	{
		while(running_)
		{
			// Renders previous frame to window and poll for new event
			if(window_manager_->Update())
			{
				running_ = false;
				break;
			}

			// Update the camera
			active_camera_->Update();

			// Update cef
			cef_adaptor_->Update();

			// Render to the back buffer
			rendering_engine_->Render(*active_camera_);
		}
	}

	// initialise components of an entity along with its sub-entities
	void Controller::InitEntity(Entity & entity)
	{
		for(unowned_ptr<SpriteRenderer> comp : entity.GetComponents<SpriteRenderer>())
		{
			// initialise the component
			comp->Init();
			DEBUG_LOG("Initialised SpriteRenderer");

			// then add the component to the render pool
			rendering_engine_->GetRenderPool().AddSpriteRenderer(entity.GetGlobalTransform(), comp);
		}

		for(unowned_ptr<TileRenderer> comp : entity.GetComponents<TileRenderer>())
		{
			// initialise the component
			comp->Init();
			DEBUG_LOG("Initialised TileRenderer");

			// then add the component to the render pool
			rendering_engine_->GetRenderPool().AddTileRenderer(entity.GetGlobalTransform(), comp);
		}

		// initialise all sub entities
		for(auto const & sub_entity : entity.GetEntities())
		{
			InitEntity(*sub_entity);
		}
	}

	// Called on framebuffer resize
	void Controller::OnFramebufferResize(int width, int height)
	{
		rendering_engine_->SetFramebufferSize(width, height);
		cef_adaptor_->SetFramebufferSize(width, height);
	}

	// Called on user input change
	void Controller::OnInputChange(EInputType type, bool triggered)
	{
		input_manager_->SetInputType(type, triggered);
	}

	// Called on mouse position change
	void Controller::OnMousePosChange(double x, double y)
	{
		input_manager_->SetMousePos(x, y);
	}
}
