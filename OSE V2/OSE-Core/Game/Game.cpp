#include "stdafx.h"
#include "Game.h"
#include "Scene/Scene.h"
#include "OSE-Core/Resources/ResourceManager.h"
#include "OSE-Core/Project/Project.h"
#include "OSE-Core/Project/ProjectLoader.h"
#include "OSE-Core/Windowing/WindowManager.h"
#include "OSE-Core/Rendering/RenderingEngine.h"
#include "OSE-Core/Scripting/ScriptingEngine.h"
#include "OSE-Core/Entity/Entity.h"
#include "OSE-Core/Entity/Component/Component.h"
#include "OSE-Core/Entity/Component/SpriteRenderer.h"
#include "OSE-Core/Entity/Component/TileRenderer.h"
#include "OSE-Core/Entity/Component/CustomComponent.h"
#include "OSE-Core/Resources/Custom Data/CustomObject.h"
#include "OSE-Core/EngineReferences.h"
#include "OSE-Core/Windowing/WindowingFactory.h"
#include "OSE-Core/Rendering/RenderingFactory.h"
#include "OSE-Core/Scripting/ScriptingFactory.h"

namespace ose
{
	Game::Game() : SceneSwitchManager(), EntityList(), InputManager()
	{
		this->running_ = false;

		///this->render_pool_ = std::move(RenderPoolFactories[0]());
		///this->thread_manager_ = std::make_unique<ThreadManager>(*render_pool_);
		
		this->window_manager_ = WindowingFactories[0]->NewWindowManager();
		this->window_manager_->NewWindow(1);
		int fbwidth { this->window_manager_->GetFramebufferWidth() };
		int fbheight { this->window_manager_->GetFramebufferHeight() };

		this->rendering_engine_ = std::move(RenderingFactories[0]->NewRenderingEngine());
		this->window_manager_->SetEngineReferences(rendering_engine_.get(), this);
		this->rendering_engine_->SetProjectionModeAndFbSize(EProjectionMode::ORTHOGRAPHIC, fbwidth, fbheight);

		this->scripting_engine_ = ScriptingFactories[0]->NewScriptingEngine();

		this->time_.Init(this->window_manager_->GetTimeSeconds());
	}

	Game::~Game() noexcept {}

	// Called upon a project being activated
	// Project is activated upon successful load
	// Only one project can be active at a time
	void Game::OnProjectActivated(Project & project)
	{
		// Clear the input manager of inputs from previous projects then apply the default project inputs
		ClearInputs();
		ApplyInputSettings(project.GetInputSettings());
	}

	// Called upon a project being deactivated
	// Project is deactivated when a new project is loaded
	void Game::OnProjectDeactivated(Project & project)
	{
		// TODO
	}

	// Called upon a scene being activated
	// Depending on switch manager, could be multiple active scenes
	void Game::OnSceneActivated(Scene & scene)
	{
		// IMPORTANT - the following code can only be run on the same thread as the render context

		// create GPU memory for the new resources
		project_->CreateGpuResources();

		// Activate entities in the scene iff they are set to enabled
		for(auto const & entity : scene.GetEntities())
		{
			// Ensure the entity has a reference to the game to allow activation/deactivation/updating
			entity->SetGameReference(this);
			// Activate the entity if it is marked as enabled
			if(entity->IsEnabled())
				OnEntityActivated(*entity);
		}
	}

	// Called upon a scene being deactivated
	// Depending on switch manager, could be multiple active scenes
	void Game::OnSceneDeactivated(Scene & scene)
	{
		// create GPU memory for the new render objects
		for(auto const & entity : scene.GetEntities())
		{
			// NOTE - Do not need to set game reference to nullptr since entity still has control over activation
			// Deactivate the entity if it is currently enabled
			if(entity->IsEnabled())
				OnEntityDeactivated(*entity);
		}
	}

	void Game::StartGame()
	{
		if(!running_)
		{
			running_ = true;
			RunGame();
		}
		else
		{
			LOG("Error: cannot start game, game is already running");
		}
	}

	void Game::RunGame()
	{
		// Initialise the user scripts after the game is initialised but before the game starts
		scripting_engine_->Init(this);

		while(running_)
		{
			// renders previous frame to window and poll for new event
			window_manager_->Update();

			// update all timing variables
			time_.Update(window_manager_->GetTimeSeconds());

			// execute developer created scripts
			scripting_engine_->Update();

			// render to the back buffer
			rendering_engine_->Update();

			// TODO - Remove once proper FPS display is implemented
			window_manager_->SetTitle(std::to_string(time_.GetFps()));
		}
	}

	// Activate an entity along with activated sub-entities
	void Game::OnEntityActivated(Entity & entity)
	{
		DEBUG_LOG("Activating Entity " << entity.GetName());

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

		for(unowned_ptr<CustomComponent> comp : entity.GetComponents<CustomComponent>())
		{
			// initialise the component
			comp->Init();
			DEBUG_LOG("Initialised CustomComponent");

			// then add the component to the script pool
			scripting_engine_->GetScriptPool().AddCustomComponent(entity, comp);
		}

		// Activate the sub entities iff they are set to active
		for(auto const & sub_entity : entity.GetEntities())
		{
			// Ensure the entity has a reference to the game to allow activation/deactivation/updating
			sub_entity->SetGameReference(this);
			// Activate the entity if it is marked as enabled
			if(sub_entity->IsEnabled())
				OnEntityActivated(*sub_entity);
		}
	}

	// Deactivate an entity along with all its sub-entities
	void Game::OnEntityDeactivated(Entity & entity)
	{
		DEBUG_LOG("De-activating Entity " << entity.GetName());

		// TODO - Remove components from engines

		// Deactivate the sub entities iff they are enabled (if disabled, they are also inactive)
		for(auto const & sub_entity : entity.GetEntities())
		{
			// Remove the game reference from the entity since it no longer has control over its own activation
			sub_entity->SetGameReference(nullptr);
			// Deactivate the entity if it is currently enabled
			if(sub_entity->IsEnabled())
				OnEntityDeactivated(*sub_entity);
		}
	}
	
	// Load a custom data file
	std::unique_ptr<CustomObject> Game::LoadCustomDataFile(std::string const & path)
	{
		return project_loader_->LoadCustomDataFile(path);
	}

	// Save a custom data file
	void Game::SaveCustomDataFile(std::string const & path, CustomObject const & object)
	{
		project_loader_->SaveCustomDataFile(path, object);
	}
}

