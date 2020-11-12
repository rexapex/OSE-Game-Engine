#pragma once

namespace ose
{
	class Project;
	struct ProjectInfo;
	struct ProjectSettings;
	class Scene;
	class Chunk;
	class Tag;
	class Entity;
	struct CustomObject;
	struct InputSettings;
	struct ControlSettings;

	// Loads developer written project files into the engine
	class ProjectLoader
	{
	public:
		ProjectLoader();
		virtual ~ProjectLoader() noexcept;

		static std::string GetProjectFileFormat(const std::string & project_path);

		// Loads all properties of the project, but does NOT load a scene
		// Returns a unique_ptr to the project instance
		virtual uptr<Project> LoadProject(const std::string & project_path) = 0;

		// Loads the manifest including: version, name, etc.
		virtual uptr<ProjectInfo> LoadProjectManifest(const std::string & project_path) = 0;

		// Loads the scene decleration file (file is essentially just a map from scene_name to scene_path)
		virtual uptr<std::map<std::string, std::string>> LoadSceneDeclerations(const std::string & project_path) = 0;

		// Loads the hierarchy of all tags in the game (entities specified in entity_prefab or scene file can be given tags)
		// Returns the root tag of the hierarchy
		virtual uptr<Tag> LoadTagDefinitions(const std::string & project_path) = 0;

		// Loads settings including: physics & rendering engine settings
		virtual ProjectSettings LoadProjectSettings(const std::string & project_path) = 0;

		// Loads game developer defined default input controls
		virtual InputSettings LoadInputSettings(const std::string & project_path) = 0;

		// Loads the control scripts which persist through all scenes
		virtual ControlSettings LoadPersistentControls(const std::string & project_path) = 0;

		// Loads a scene file defined by the game developer
		virtual uptr<Scene> LoadScene(Project const & project, const std::string & scene_name) = 0;

		// Loads a chunk of a loaded scene
		virtual void LoadChunk(Chunk & chunk, Project const & project) = 0;

		// Loads an entity prefab which can be used multiple times
		virtual uptr<Entity> LoadEntityPrefab(const std::string & prefab_path, const Project & project) = 0;

		// Load a custom data file into a custom object
		virtual uptr<CustomObject> LoadCustomDataFile(const std::string & path) = 0;

		// Save a custom object into a custom data file
		virtual void SaveCustomDataFile(const std::string & path, CustomObject const & object) = 0;
	};
}
