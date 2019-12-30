#pragma once

namespace ose
{
	namespace game
	{
		class Scene;
		class Tag;
	}
	namespace entity
	{
		class Entity;
	}
	namespace resources
	{
		struct CustomObject;
	}
}
	namespace input
	{
		struct InputSettings;
	}
}

namespace ose::project
{
	class Project;
	struct ProjectInfo;

	// Loads developer written project files into the engine
	class ProjectLoader
	{
	public:
		ProjectLoader();
		virtual ~ProjectLoader() noexcept;

		static std::string GetProjectFileFormat(const std::string & project_name);

		// Loads all properties of the project, but does NOT load a scene
		// Returns a unique_ptr to the project instance
		virtual std::unique_ptr<Project> LoadProject(const std::string & project_name) = 0;

		// Loads the manifest including: version, name, etc.
		virtual std::unique_ptr<ProjectInfo> LoadProjectManifest(const std::string & project_path) = 0;

		// Loads the scene decleration file (file is essentially just a map from scene_name to scene_path)
		virtual std::unique_ptr<std::map<std::string, std::string>> LoadSceneDeclerations(const std::string & project_path) = 0;

		// Loads the hierarchy of all tags in the game (entities specified in entity_prefab or scene file can be given tags)
		// Returns the root tag of the hierarchy
		virtual std::unique_ptr<ose::game::Tag> LoadTagDefinitions(const std::string & project_path) = 0;

		// Loads settings including: physics & rendering engine default settings
		virtual void LoadProjectSettings(const std::string & project_path) = 0;

		//Loads game developer defined default input controls
		virtual ose::input::InputSettings LoadInputSettings(const std::string & project_path) = 0;

		// Loads a scene file defined by the game developer
		virtual std::unique_ptr<ose::game::Scene> LoadScene(const Project & project, const std::string & scene_name) = 0;

		// Loads an entity prefab which can be used multiple times
		virtual std::unique_ptr<ose::entity::Entity> LoadEntityPrefab(const std::string & prefab_path, const Project & project) = 0;

		// Load a custom data file into a custom object
		virtual std::unique_ptr<ose::resources::CustomObject> LoadCustomDataFile(const std::string & path) = 0;
	};
}
