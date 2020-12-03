#pragma once
#include "OSE-Core/Project/ProjectLoader.h"
#include "OSE-Core/Game/Tagging/TagSettings.h"

namespace rapidxml
{
	template <class Ch = char> class xml_document;
	template <class Ch = char> class xml_node;
}

namespace ose
{
	struct InputSettings;
	struct ControlSettings;
	struct ProjectSettings;

	class EntityList;
}

namespace ose::project
{
	//Loads projects from xml text files
	class ProjectLoaderXML : public ProjectLoader
	{
	public:
		ProjectLoaderXML();
		virtual ~ProjectLoaderXML() noexcept;
		
		uptr<Project> LoadProject(std::string const & project_name);
		uptr<ProjectInfo> LoadProjectManifest(std::string const & project_path);

		std::unique_ptr<std::map<std::string, std::string>> LoadSceneDeclerations(const std::string & project_path);
		TagSettings LoadTagSettings(const std::string & project_path);

		ProjectSettings LoadProjectSettings(std::string const & project_path);
		InputSettings LoadInputSettings(std::string const & project_path);
		ControlSettings LoadPersistentControls(std::string const & project_path);

		uptr<Scene> LoadScene(Project const & project, std::string const & scene_name);
		void LoadChunk(Chunk & chunk, Project const & project);
		uptr<Entity> LoadEntityPrefab(std::string const & prefab_path, Project const & project);

		uptr<CustomObject> LoadCustomDataFile(std::string const & path);
		void SaveCustomDataFile(std::string const & path, CustomObject const & object);

	private:
		// Parse the XML file at 'path' and store the contents in 'content'
		// NOTE - contents must remain in memory while accessing doc as it used in-place parsing
		// Returns {uptr<rapidxml::xml_document<>>} Pointer to the parsed document
		uptr<rapidxml::xml_document<>> LoadXmlFile(std::string const & path, std::string & contents);

		// Parse the XML of an entity
		// If parent != nullptr, the new entity is added to the parent and the return value is nullptr
		// If parent == nullptr, the new entity is returned
		uptr<Entity> ParseEntity(EntityList * parent, rapidxml::xml_node<> * entity_node,
				std::unordered_map<std::string, std::string> & aliases, Project const & project);

		// Parse the XML of a transformable (entity / chunk)
		void ParseTransform(glm::vec3 & out_translation, glm::vec3 & out_scale, glm::vec3 & out_rotation, rapidxml::xml_node<> * transform_node);
		
		void ParseTag(std::vector<TagSettings::TagNode> & tags, rapidxml::xml_node<> * tag_node);
		void ParseResources(rapidxml::xml_node<> * resources_node, std::unordered_map<std::string, std::string> & aliases, const Project & project);
		ControlSettings ParseControls(rapidxml::xml_node<> * controls_node);

		uptr<CustomObject> ParseCustomObject(rapidxml::xml_node<> * obj_node);
		void SaveCustomDataObject(rapidxml::xml_document<> & doc, CustomObject const & object, rapidxml::xml_node<> * parent = nullptr, std::string name = "");

		std::string const file_extension = ".xml";
	};
}

