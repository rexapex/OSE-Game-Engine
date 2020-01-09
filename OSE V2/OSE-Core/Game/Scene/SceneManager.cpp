#include "stdafx.h"
#include "Scene.h"
#include "SceneManager.h"
#include "OSE-Core/Project/Project.h"
#include "OSE-Core/Project/ProjectLoader.h"
#include "OSE-Core/EngineReferences.h"
#include "OSE-Core/Project/ProjectLoaderFactory.h"

namespace ose
{
	SceneManager::SceneManager()
	{
		this->project_loader_ = std::move(ProjectLoaderFactories[0]->NewProjectLoader());
		this->scene_switch_mode_ = ESceneSwitchMode::REMOVE_ALL_ON_SWITCH;
	}

	SceneManager::~SceneManager() noexcept
	{

	}

	// loads the project specified (does not load any scenes)
	// throws std::exception if the project could not be loaded
	// throws std::invalid_argument if the project FILE_FORMAT decleration file does not exist
	void SceneManager::LoadProject(const std::string & proj_name)
	{
		// before project can be loaded, the file format of the project files must de determined
		std::string proj_file_format = ProjectLoader::GetProjectFileFormat(proj_name);

		if(proj_file_format == "XML") {
			if(project_ != nullptr)
				OnProjectDeactivated(*project_);

			project_ = project_loader_->LoadProject(proj_name);

			if(project_ == nullptr)
				throw std::exception("Error: Could not load Project");

			OnProjectActivated(*project_);
		} else {
			throw std::exception("Error: Unknown project file type");
		}
	}

	// Loads the scene into memory
	// If the scene is already loaded, nothing happens
	// Throws std::exception if no project has been loaded
	// Throws std::exception if the scene could not be loaded
	void SceneManager::LoadScene(const std::string & scene_name)
	{
		// first, check that the scene actually exists
		auto map = project_->GetSceneNamesToPathMap();
		auto pos = map.find(scene_name);
		if(pos == map.end()) {
			throw std::invalid_argument("Error: The Scene " + scene_name + " does not exist in the current Project");
		}

		auto index = this->loaded_scenes_.find(scene_name);

		// Load the scene if it hasn't already been loaded
		if(index == loaded_scenes_.end())
		{
			// load the scene using the project loader
			auto scene = project_loader_->LoadScene(*project_, scene_name);

			// scene pointer will be nullptr if no scene exists with name scene_name or the scene file failed to load
			if(!scene)
			{
				throw std::exception("Error: Failed to load scene");
			}
			scene->Print();

			loaded_scenes_.emplace(scene_name, std::move(scene));
			//scene unique_ptr is no longer usable since its pointer has been moved
		}
	}


	void SceneManager::UnloadScene(const std::string & scene_name)
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


	void SceneManager::UnloadAllLoadedScenes()
	{
		this->loaded_scenes_.clear();		//easy... I hope
	}


	void SceneManager::SetActiveScene(const std::string & scene_name)
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
				//remove all loaded scenes then add the active scene to the loaded scenes list
				this->loaded_scenes_.clear();
				if(active_scene_)
					this->loaded_scenes_.emplace(active_scene_->GetName(), std::move(active_scene_));
				break;
			}
			case ESceneSwitchMode::REMOVE_NONE_ON_SWITCH:
			{
				//add the active scene to the loaded scenes list so all other scenes are now in loaded list
				if(active_scene_)
					this->loaded_scenes_.emplace(active_scene_->GetName(), std::move(active_scene_));
				break;
			}
			//no case neeeded for -> ESceneSwitchMode::REMOVE_ACTIVE_ON_SWITCH
			//as active_scene_ is auto deleted when new scene is made active
			}

			this->active_scene_ = std::move(new_scene);		//finally, move the new_scene to the active_scene pointer
		}

		// Activate the scene
		OnSceneActivated(*this->active_scene_);
	}
}
