#include "stdafx.h"
#include "ProjectLoaderXML.h"

namespace origami_sheep_engine
{
	ProjectLoaderXML::ProjectLoaderXML() : ProjectLoader()
	{

	}


	ProjectLoaderXML::~ProjectLoaderXML() noexcept
	{

	}


	std::unique_ptr<rapidxml::xml_document<>> ProjectLoaderXML::loadXMLFile(const std::string & path, std::string & contents)
	{
		//load the xml string
		try
		{
			FileHandlingUtil::loadTextFile(path, contents);
		}
		catch(const std::exception & e)
		{
			//error occurred, therefore, return an empty project info stub
			std::cerr << "FileHandlingUtil::load_text_file -> " << e.what() << std::endl;
			throw e;
		}

		//load the xml dom tree
		using namespace rapidxml;
		std::unique_ptr<xml_document<>> doc = std::make_unique<xml_document<>>();
		char * c_str = const_cast<char *>(contents.c_str());
		doc->parse<0>(c_str);

		return doc;
	}


	std::unique_ptr<Project> ProjectLoaderXML::loadProject(const std::string & project_name)
	{
		//TODO - FIND DOCUMENT DIRECTORY FOR WINDOWS, MAC & LINUX
		std::string project_path = "D:/James/Documents/Origami_Sheep_Engine/Projects/" + project_name;

		//first, load the manifest
		std::unique_ptr<ProjectInfo> manifest = loadProjectManifest(project_path);

		//then, load the scene declerations
		std::unique_ptr<std::map<std::string, std::string>> scene_declerations = loadSceneDeclerations(project_path);

		//then, load the tag definitions
		loadTagDefinitions(project_path);

		//finally, construct a new project instance
		std::unique_ptr<Project> proj = std::make_unique<Project>(project_path, *manifest, *scene_declerations);

		return proj;
	}


	std::unique_ptr<ProjectInfo> ProjectLoaderXML::loadProjectManifest(const std::string & project_path)
	{
		using namespace rapidxml;

		std::unique_ptr<xml_document<>> doc;
		std::string contents;

		try
		{
			doc = loadXMLFile(project_path + "/info.xml", contents);
		}
		catch(const std::exception & e)
		{
			return std::make_unique<ProjectInfo>(std::move(ProjectInfo {"UNKNOWN", "UNKNOWN", "UNKNOWN", "UNKNOWN", "UNKNOWN"}));
		}

		std::cerr << "**********  Project Manifest  **********" << std::endl;

		//parse the xml
		//valid project manifest file should contain: name, version, date_created, date_modified
		xml_node<> * name_node = doc->first_node("name");
		std::string name = (name_node ? name_node->value() : "UNKNOWN");
		std::cerr << "name: " << name << std::endl;

		xml_node<> * engine_version_node = doc->first_node("engine_version");
		std::string engine_version = (engine_version_node ? engine_version_node->value() : "UNKNOWN");
		std::cerr << "engine_version: " << engine_version << std::endl;

		xml_node<> * game_version_node = doc->first_node("game_version");
		std::string game_version = (game_version_node ? game_version_node->value() : "UNKNOWN");
		std::cerr << "game_version: " << game_version << std::endl;

		xml_node<> * date_created_node = doc->first_node("date_created");
		std::string date_created = (date_created_node ? date_created_node->value() : "UNKNOWN");
		std::cerr << "date_created: " << date_created << std::endl;

		xml_node<> * date_modified_node = doc->first_node("date_modified");
		std::string date_modified = (date_modified_node ? date_modified_node->value() : "UNKNOWN");
		std::cerr << "date_modified: " << date_modified << std::endl;

		std::cerr << std::endl;

		//construct and return a new ProjectInfo instance
		return std::make_unique<ProjectInfo>(std::move(ProjectInfo {name, engine_version, game_version, date_created, date_modified}));
	}


	std::unique_ptr<std::map<std::string, std::string>> ProjectLoaderXML::loadSceneDeclerations(const std::string & project_path)
	{
		using namespace rapidxml;

		std::unique_ptr<std::map<std::string, std::string>> name_to_path_map = std::make_unique<std::map<std::string, std::string>>();
		std::unique_ptr<xml_document<>> doc;
		std::string contents;
		
		try
		{
			doc = loadXMLFile(project_path + "/scene_declerations.xml", contents);
		}
		catch(const std::exception & e)
		{
			return name_to_path_map;
		}

		std::cerr << "**********  Scene Declerations  **********" << std::endl;

		for(xml_node<> * scene_node = doc->first_node("scene"); scene_node; scene_node = scene_node->next_sibling())
		{
			//for each scene decleration, add an entry to the map
			xml_attribute<> * name_attrib = scene_node->first_attribute("name");
			xml_attribute<> * path_attrib = scene_node->first_attribute("path");

			if(name_attrib && path_attrib)
			{
				//map name to path
				name_to_path_map->insert({name_attrib->value(), path_attrib->value()});
				std::cerr << "Scene {name: " << name_attrib->value() << ", path: " << path_attrib->value() << "}" << std::endl;
			}
		}

		std::cerr << std::endl;

		return name_to_path_map;
	}


	void ProjectLoaderXML::loadTagDefinitions(const std::string & project_path)
	{
		using namespace rapidxml;

		std::unique_ptr<xml_document<>> doc;
		std::string contents;

		try
		{
			doc = loadXMLFile(project_path + "/tags.xml", contents);
		}
		catch(const std::exception & e)
		{
			return;
		}

		std::cerr << "**********  Tag Definitions  **********" << std::endl;

		auto root_tag_node = doc->first_node("tag");
		auto root_tag_name_attrib = (root_tag_node ? root_tag_node->first_attribute("name") : nullptr);

		//every tag hierarchy should be contained in a root_tag with name ""
		std::unique_ptr<Tag> root_tag = std::make_unique<Tag>(root_tag_name_attrib ? root_tag_name_attrib->value() : "");

		for(auto tag_node = doc->first_node("tag"); tag_node; tag_node = tag_node->next_sibling())
		{
			parseTag(root_tag->get_sub_tags(), tag_node);
		}

		std::cerr << std::endl;
	}


	void ProjectLoaderXML::parseTag(std::vector<Tag> & tags, rapidxml::xml_node<> * tag_node)
	{
		using namespace rapidxml;

		auto name_attrib = tag_node->first_attribute("name");
		const std::string & name = (name_attrib ? name_attrib->value() : "");

		std::cerr << "tag -> name: " << name << std::endl;

		//add the tags to the tags list
		tags.emplace_back(name);
		auto & new_tag = tags.back();
		auto & sub_list = new_tag.get_sub_tags();

		//parse any sub-tags
		for(auto sub_tag_node = tag_node->first_node("tag"); sub_tag_node; sub_tag_node = sub_tag_node->next_sibling())
		{
			parseTag(sub_list, sub_tag_node);
		}
	}


	void ProjectLoaderXML::loadProjectSettings(const std::string & project_path)
	{
		//TODO
	}


	void ProjectLoaderXML::loadInputSettings(const std::string & project_path)
	{
		//TODO
	}


	std::unique_ptr<Scene> ProjectLoaderXML::loadScene(const Project & project, const std::string & scene_name)
	{
		using namespace rapidxml;

		std::unique_ptr<xml_document<>> doc;
		std::string contents;
		std::string scene_path;

		auto map = project.get_scene_names_to_path_map();
		auto pos = map.find(scene_name);
		if(pos == map.end())
		{
			std::cerr << "ProjectLoaderXML::loadScene -> ERROR: " + scene_name + " does not exist" << std::endl;
			return nullptr;
		}
		else
		{
			scene_path = project.get_project_path() + "/" + pos->second + ".xml";
		}

		try
		{
			doc = loadXMLFile(scene_path, contents);
		}
		catch(const std::exception & e)
		{
			return nullptr;
		}

		auto scene_node = doc->first_node("scene");
		auto scene_name_attrib = (scene_node ? scene_node->first_attribute("name") : nullptr);
		auto entities_node = scene_node->first_node("entities");
		auto resources_node = scene_node->first_node("resources");

		std::unique_ptr<Scene> scene = std::make_unique<Scene>(scene_name_attrib ? scene_name_attrib->value() : scene_name);

		//get the prefabs required for this scene
		std::map<std::string, std::string> prefab_names_to_path;
		for(auto prefab_node = resources_node->first_node("prefab"); prefab_node; prefab_node = prefab_node->next_sibling())
		{
			auto prefab_name_attrib = prefab_node->first_attribute("name");
			auto prefab_path_attrib = prefab_node->first_attribute("path");
			
			if(prefab_name_attrib && prefab_path_attrib)
			{
				auto prefab_name = prefab_name_attrib->value();
				auto prefab_path = prefab_path_attrib->value();
				auto pos = prefab_names_to_path.find(prefab_name);

				//check if the prefab name has not been used
				if(pos == prefab_names_to_path.end() && prefab_name != "" && prefab_path != "")
				{
					//and if so, then load the corresponding prefab file and add it to the map
					prefab_names_to_path.insert({prefab_name, prefab_path});
				}
			}
		}
		
		//load the scene's entities
		for(auto entity_node = entities_node->first_node("entity"); entity_node; entity_node = entity_node->next_sibling())
		{
			parseEntity(scene->get_entities(), entity_node);
		}

		return scene;
	}


	void ProjectLoaderXML::loadEntityPrefab(std::vector<Entity> & prefabs, const std::string & prefab_path, std::map<std::string, std::string> & prefab_names_to_path)
	{
		using namespace rapidxml;

		std::unique_ptr<xml_document<>> doc;
		std::string contents;

		try
		{
			doc = loadXMLFile(prefab_path, contents);
		}
		catch(const std::exception & e)
		{
			return;
		}

		auto resources_node = doc->first_node("resources");
		auto entity_node = doc->first_node("entity");

		parseEntity(prefabs, entity_node);
	}


	void ProjectLoaderXML::parseEntity(std::vector<Entity> & entities, rapidxml::xml_node<> * entity_node)
	{
		using namespace rapidxml;

		auto name_attrib = entity_node->first_attribute("name");
		const std::string & name = (name_attrib ? name_attrib->value() : "");

		auto tag_attrib = entity_node->first_attribute("tag");
		const std::string & tag = (tag_attrib ? tag_attrib->value() : "");

		auto prefab_attrib = entity_node->first_attribute("prefab");
		const std::string & prefab = (prefab_attrib ? prefab_attrib->value(): "");

		//add the entity to the entities list
		try
		{
			entities.emplace_back(0, name, tag, prefab);
		}
		catch(const std::exception & e)
		{
			//if adding the entity fails, exit the function to avoid overwriting previous entities
			std::cerr << "ERROR: failed to load entity, " << name << std::endl;
			return;
		}

		//get the newly added entity
		auto & new_entity = entities.back();
		auto & sub_list = new_entity.get_sub_entities();

		//parse the components of the new entity
		//mesh components
		for(auto component_node = entity_node->first_node("mesh"); component_node; component_node = component_node->next_sibling())
		{
			//has name & path attributes
			auto name_attrib = component_node->first_attribute("name");
			auto path_attrib = component_node->first_attribute("path");
			new_entity.mesh_ = std::make_unique<MeshFilter>(std::move(MeshFilter {(name_attrib ? name_attrib->value() : ""), (path_attrib ? path_attrib->value() : "")}));
		}

		//parse any sub-entities
		for(auto sub_entity_node = entity_node->first_node("entity"); sub_entity_node; sub_entity_node = sub_entity_node->next_sibling())
		{
			parseEntity(sub_list, sub_entity_node);
		}
	}
}
