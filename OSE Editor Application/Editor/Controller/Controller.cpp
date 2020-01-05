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

namespace ose::editor
{
	Controller::Controller()
	{
		this->window_manager_ = WindowingFactories[0]->NewWindowManager();
		this->window_manager_->NewWindow(1);
		int fbwidth { this->window_manager_->GetFramebufferWidth() };
		int fbheight { this->window_manager_->GetFramebufferHeight() };

		this->input_manager_ = std::make_unique<InputManager>();

		this->rendering_engine_ = std::move(RenderingFactories[0]->NewRenderingEngine());
		this->window_manager_->SetEngineReferences(rendering_engine_.get(), input_manager_.get());
		this->rendering_engine_->SetProjectionModeAndFbSize(EProjectionMode::ORTHOGRAPHIC, fbwidth, fbheight);
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

	// Render the active scene
	void Controller::Render()
	{
		this->rendering_engine_->Update();
		this->window_manager_->Update();
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
}
