#pragma once
#include "OSE-Core/Project/ProjectLoader.h"

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
}

namespace ose::project
{
	//Loads projects from xml text files
	class ProjectLoaderXML : public ProjectLoader
	{
	public:
		ProjectLoaderXML();
		virtual ~ProjectLoaderXML() noexcept;

		std::unique_ptr<Project> LoadProject(const std::string & project_name);
		std::unique_ptr<ProjectInfo> LoadProjectManifest(const std::string & project_path);

		std::unique_ptr<std::map<std::string, std::string>> LoadSceneDeclerations(const std::string & project_path);
		std::unique_ptr<Tag> LoadTagDefinitions(const std::string & project_path);

		ProjectSettings LoadProjectSettings(const std::string & project_path);
		InputSettings LoadInputSettings(const std::string & project_path);
		ControlSettings LoadPersistentControls(const std::string & project_path);

		std::unique_ptr<Scene> LoadScene(const Project & project, const std::string & scene_name);
		std::unique_ptr<Entity> LoadEntityPrefab(const std::string & prefab_path, const Project & project);

		std::unique_ptr<CustomObject> LoadCustomDataFile(const std::string & path);
		void SaveCustomDataFile(const std::string & path, CustomObject const & object);

	private:
		//Parse the XML file at 'path' and store the contents in 'content'
		//NOTE - contents must remain in memory while accessing doc as it used in-place parsing
		//@returns {std::unique_ptr<rapidxml::xml_document<>>} Pointer to the parsed document
		std::unique_ptr<rapidxml::xml_document<>> LoadXmlFile(const std::string & path, std::string & contents);

		std::unique_ptr<Entity> ParseEntity(rapidxml::xml_node<> * entity_node,
				std::unordered_map<std::string, std::string> & aliases, const Project & project);
		
		void ParseTag(std::vector<Tag> & tags, rapidxml::xml_node<> * tag_node);
		void ParseResources(rapidxml::xml_node<> * resources_node, std::unordered_map<std::string, std::string> & aliases, const Project & project);
		ControlSettings ParseControls(rapidxml::xml_node<> * controls_node);

		std::unique_ptr<CustomObject> ParseCustomObject(rapidxml::xml_node<> * obj_node);
		void SaveCustomDataObject(rapidxml::xml_document<> & doc, CustomObject const & object, rapidxml::xml_node<> * parent = nullptr, std::string name = "");

		const std::string file_extension = ".xml";
	};
}

