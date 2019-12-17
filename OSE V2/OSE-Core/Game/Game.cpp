#include "stdafx.h"
#include "Game.h"
#include "Scene/Scene.h"
#include "OSE-Core/Resources/ResourceManager.h"
#include "OSE-Core/Project/Project.h"
#include "OSE-Core/Project/ProjectLoader.h"
#include "OSE-Core/Windowing/WindowManager.h"
#include "OSE-Core/Rendering/RenderingEngine.h"
#include "OSE-Core/Entity/Entity.h"
#include "OSE-Core/Entity/Component/Component.h"
#include "OSE-Core/Entity/Component/SpriteRenderer.h"
#include "OSE-Core/Entity/Component/TileRenderer.h"
#include "OSE-Core/Engine/EngineTaskPool.h"
#include "OSE-Core/EngineReferences.h"
#include "OSE-Core/Windowing/WindowingFactory.h"
#include "OSE-Core/Rendering/RenderingFactory.h"

using namespace ose::project;
using namespace ose::windowing;
using namespace ose::rendering;
using namespace ose::resources;
using namespace ose::engine;
using namespace ose::entity;

namespace ose::game
{
	Game::Game() : SceneSwitchManager(), EntityList()
	{
		this->running_ = false;

		///this->render_pool_ = std::move(RenderPoolFactories[0]());
		///this->thread_manager_ = std::make_unique<ThreadManager>(*render_pool_);
		
		this->window_manager_ = WindowingFactories[0]->NewWindowManager();
		this->window_manager_->NewWindow(1);
		int fbwidth { this->window_manager_->GetFramebufferWidth() };
		int fbheight { this->window_manager_->GetFramebufferHeight() };

		this->rendering_engine_ = std::move(RenderingFactories[0]->NewRenderingEngine());
		this->window_manager_->SetEngineReferences(rendering_engine_.get());
		this->rendering_engine_->SetProjectionModeAndFbSize(EProjectionMode::ORTHOGRAPHIC, fbwidth, fbheight);

		this->time_.Init(this->window_manager_->GetTimeSeconds());
	}

	Game::~Game() noexcept {}

	// Called upon a scene being activated
	// Depending on switch manager, could be multiple active scenes
	void Game::OnSceneActivated(Scene & scene)
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
		// get current time in seconds
		// TODO - should I use this or window_manager_ timing
		// time_t t = time(0);
		///EditorImpl editor_temp (*this->window_manager_);
		///DataObject stub;

		while(running_)
		{
			// renders previous frame to window and poll for new event
			window_manager_->Update();

			// update all timing variables
			time_.Update(window_manager_->GetTimeSeconds());

			// render to the back buffer
			rendering_engine_->Update();
			///thread_manager_->ProcessRenderTasks();

			///editor_temp.update(stub);
		}
	}

	// initialise components of an entity along with its sub-entities
	void Game::InitEntity(const Entity & entity)
	{
		// for each sprite renderer component of the entity		
		for(ose::unowned_ptr<SpriteRenderer> comp : entity.GetComponents<SpriteRenderer>())
		{
			// initialise the component
			comp->Init();
			DEBUG_LOG("Initialised SpriteRenderer");

			// then add the component to the render pool
			rendering_engine_->GetRenderPool().AddSpriteRenderer(entity.GetGlobalTransform(), comp);
		}

		for(ose::unowned_ptr<TileRenderer> comp : entity.GetComponents<TileRenderer>())
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

