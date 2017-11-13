#include "stdafx.h"
#include "Game.h"

namespace origami_sheep_engine
{
	Game::Game()
	{
		std::cerr << "Hello from LUbuntu!" << std::endl;
		std::cerr << "Hello from Windows 10!" << std::endl;
		this->project_loader_ = std::make_unique<ProjectLoaderXML>();
		this->scene_switch_mode_ = ESceneSwitchMode::REMOVE_ALL_ON_SWITCH;
	}


	Game::~Game() noexcept
	{

	}


	Game::Game(Game && other) noexcept : project_(std::move(other.project_)), project_loader_(std::move(other.project_loader_)),
										 active_scene_(std::move(other.active_scene_))
	{

	}


	Game & Game::operator=(Game && other) noexcept
	{
		this->project_ = std::move(other.project_);
		this->project_loader_ = std::move(other.project_loader_);
		this->active_scene_ = std::move(other.active_scene_);
		return *this;
	}


	void Game::loadProject(const std::string & proj_name)
	{
		this->project_ = this->project_loader_->loadProject(proj_name);
	}


	void Game::loadScene(const std::string & scene_name)
	{
		auto scene = this->project_loader_->loadScene(*project_, scene_name);
		scene->print();

		auto index = this->loaded_scenes_.find(scene_name);
		
		if(index == loaded_scenes_.end())
		{
			loaded_scenes_.insert({scene_name, std::move(scene)});
			//scene unique_ptr is no longer usable since its pointer has been moved
		}
		else
		{
			throw std::invalid_argument("Error: scene " + scene_name + " is already loaded");
		}
	}


	void Game::unloadScene(const std::string & scene_name)
	{
		auto iter = this->loaded_scenes_.find(scene_name);

		if(iter == loaded_scenes_.end())
		{
			throw std::invalid_argument("Error: scene " + scene_name + " is NOT loaded");
		}
		else
		{
			this->loaded_scenes_.erase(iter);
		}
	}


	void Game::unloadAllLoadedScenes()
	{
		this->loaded_scenes_.clear();		//easy
	}


	void Game::setActiveScene(const std::string & scene_name)
	{
		auto iter = this->loaded_scenes_.find(scene_name);

		if(iter == loaded_scenes_.end())
		{
			throw std::invalid_argument("Error: scene " + scene_name + " is NOT loaded");
		}
		else
		{
			std::cerr << "Setting active scene to " << scene_name << std::endl;

			auto new_scene = std::move(iter->second);		//move scene ptr to new_scene pointer
			this->loaded_scenes_.erase(iter);				//then remove the entry in the map

			//decide what to do about other scenes
			switch(this->scene_switch_mode_)
			{
				case ESceneSwitchMode::REMOVE_ALL_ON_SWITCH:
				{
					//active scene will be auto removed from active_scene_ since using unique_ptr
					this->loaded_scenes_.clear();
					break;
				}
				case ESceneSwitchMode::REMOVE_LOADED_ON_SWITCH:
				{
					//remove all loaded scene then add the active scene to the loaded scenes list
					this->loaded_scenes_.clear();
					this->loaded_scenes_.insert({active_scene_->get_name(), std::move(active_scene_)});
					break;
				}
				case ESceneSwitchMode::REMOVE_NONE_ON_SWITCH:
				{
					//add the active scene to the loaded scenes list so all other scenes are now in loaded list
					this->loaded_scenes_.insert({active_scene_->get_name(), std::move(active_scene_)});
					break;
				}
				//no case neeeded for -> ESceneSwitchMode::REMOVE_ACTIVE_ON_SWITCH
				//as active_scene_ is auto deleted when new scene is made active
			}

			this->active_scene_ = std::move(new_scene);		//finally, move the new_scene to the active_scene pointer
		}
	}
}

