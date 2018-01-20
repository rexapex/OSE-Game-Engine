#pragma once
#include "stdafx.h"
#include "Project.h"
#include "Scene.h"
#include "Tag.h"
#include "TextureMetaData.h"

namespace origami_sheep_engine
{
	//Loads developer written project files into the engine
	class ProjectLoader
	{
	public:
		ProjectLoader();
		virtual ~ProjectLoader() noexcept;

		//Loads all properties of the project, but does NOT load a scene
		//Returns a unique_ptr to the project instance
		virtual std::unique_ptr<Project> loadProject(const std::string & project_name) = 0;

		//Loads the manifest including: version, name, etc.
		virtual std::unique_ptr<ProjectInfo> loadProjectManifest(const std::string & project_path) = 0;

		//Loads the scene decleration file (file is essentially just a map from scene_name to scene_path)
		virtual std::unique_ptr<std::map<std::string, std::string>> loadSceneDeclerations(const std::string & project_path) = 0;

		//Loads the hierarchy of all tags in the game (entities specified in entity_prefab or scene file can be given tags)
		//Returns the root tag of the hierarchy
		virtual std::unique_ptr<Tag> loadTagDefinitions(const std::string & project_path) = 0;

		//Loads settings including: physics & rendering engine default settings
		virtual void loadProjectSettings(const std::string & project_path) = 0;

		//Loads game developer defined default input controls
		virtual void loadInputSettings(const std::string & project_path) = 0;

		//Loads a scene file defined by the game developer
		virtual std::unique_ptr<Scene> loadScene(const Project & project, const std::string & scene_name) = 0;

		//Loads an entity prefab which can be used multiple times
		virtual void loadEntityPrefab(std::map<std::string, Entity> & prefab_paths_to_object, const std::string & prefab_name,
															const std::string & prefab_path, const Project & project) = 0;
	};
}
