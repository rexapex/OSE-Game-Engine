#include "pch.h"
#include "ProjectLoaderXML.h"
#include "OSE-Core/Resources/FileHandlingUtil.h"
#include "OSE-Core/Project/ProjectInfo.h"
#include "OSE-Core/Project/Project.h"
#include "OSE-Core/Game/Tag.h"
#include "OSE-Core/Game/Scene/Scene.h"
#include "OSE-Core/Entity/Entity.h"
#include "OSE-Core/Resources/Texture/Texture.h"
#include "OSE-Core/Entity/SpriteRenderer.h"
#include "OSE-Core/Resources/Prefab/PrefabManager.h"
#include "OSE-Core/Resources/ResourceManager.h"
#include "Dependencies/rapidxml-1.13/rapidxml.hpp"

using namespace ose::game;
using namespace ose::entity;
using namespace ose::resources;
using namespace rapidxml;

namespace ose::project
{
	ProjectLoaderXML::ProjectLoaderXML() : ProjectLoader()
	{

	}


	ProjectLoaderXML::~ProjectLoaderXML() noexcept
	{

	}


	std::unique_ptr<rapidxml::xml_document<>> ProjectLoaderXML::LoadXmlFile(const std::string & path, std::string & contents)
	{
		//load the xml string
		try
		{
			FileHandlingUtil::LoadTextFile(path, contents);
		}
		catch(const std::exception & e)
		{
			//error occurred, therefore, return an empty project info stub
			LOG("FileHandlingUtil::load_text_file -> " << e.what());
			throw e;
		}

		//load the xml dom tree
		std::unique_ptr<xml_document<>> doc = std::make_unique<xml_document<>>();
		char * c_str = const_cast<char *>(contents.c_str());
		doc->parse<0>(c_str);

		return doc;
	}


	std::unique_ptr<Project> ProjectLoaderXML::LoadProject(const std::string & project_name)
	{
		std::string home_dir;
		FileHandlingUtil::GetHomeDirectory(home_dir);

		//TODO - FIND DOCUMENT DIRECTORY FOR MAC & LINUX - DONE - NEEDS TESTING
		//TODO - CREATE DIRECTORIES IF THEY DON'T EXIST  - DONE - NEEDS TESTING
		std::string project_path = home_dir + "/Origami_Sheep_Engine/Projects/" + project_name;
		FileHandlingUtil::CreateDirs(project_path);
		LOG("Loading Project Directory: " << project_path << std::endl);

		//first, load the manifest
		std::unique_ptr<ProjectInfo> manifest = LoadProjectManifest(project_path);

		//then, load the scene declerations
		std::unique_ptr<std::map<std::string, std::string>> scene_declerations = LoadSceneDeclerations(project_path);

		//then, load the tag definitions
		std::unique_ptr<Tag> root_tag = LoadTagDefinitions(project_path);

		//finally, construct a new project instance
		std::unique_ptr<Project> proj = std::make_unique<Project>(project_path, *manifest, *scene_declerations);

		return proj;
	}


	std::unique_ptr<ProjectInfo> ProjectLoaderXML::LoadProjectManifest(const std::string & project_path)
	{
		std::unique_ptr<xml_document<>> doc;
		std::string contents;

		try
		{
			doc = LoadXmlFile(project_path + "/info.xml", contents);
		}
		catch(const std::exception & e)
		{
			ERROR_LOG(e.what());
			return std::make_unique<ProjectInfo>(std::move(ProjectInfo {"UNKNOWN", "UNKNOWN", "UNKNOWN", "UNKNOWN", "UNKNOWN"}));
		}

		DEBUG_LOG("**********  Project Manifest  **********");

		//parse the xml
		//valid project manifest file should contain: name, version, date_created, date_modified
		xml_node<> * name_node = doc->first_node("name");
		std::string name = (name_node ? name_node->value() : "UNKNOWN");
		DEBUG_LOG("name: " << name);

		xml_node<> * engine_version_node = doc->first_node("engine_version");
		std::string engine_version = (engine_version_node ? engine_version_node->value() : "UNKNOWN");
		DEBUG_LOG("engine_version: " << engine_version);

		xml_node<> * game_version_node = doc->first_node("game_version");
		std::string game_version = (game_version_node ? game_version_node->value() : "UNKNOWN");
		DEBUG_LOG("game_version: " << game_version);

		xml_node<> * date_created_node = doc->first_node("date_created");
		std::string date_created = (date_created_node ? date_created_node->value() : "UNKNOWN");
		DEBUG_LOG("date_created: " << date_created);

		xml_node<> * date_modified_node = doc->first_node("date_modified");
		std::string date_modified = (date_modified_node ? date_modified_node->value() : "UNKNOWN");
		DEBUG_LOG("date_modified: " << date_modified);

		DEBUG_LOG("");

		//construct and return a new ProjectInfo instance
		return std::make_unique<ProjectInfo>(std::move(ProjectInfo {name, engine_version, game_version, date_created, date_modified}));
	}



	std::unique_ptr<std::map<std::string, std::string>> ProjectLoaderXML::LoadSceneDeclerations(const std::string & project_path)
	{
		std::unique_ptr<std::map<std::string, std::string>> name_to_path_map = std::make_unique<std::map<std::string, std::string>>();
		std::unique_ptr<xml_document<>> doc;
		std::string contents;
		
		try
		{
			doc = LoadXmlFile(project_path + "/scene_declerations.xml", contents);
		}
		catch(const std::exception & e)
		{
			ERROR_LOG(e.what());
			return name_to_path_map;
		}

		DEBUG_LOG("**********  Scene Declerations  **********");

		for(xml_node<> * scene_node = doc->first_node("scene"); scene_node; scene_node = scene_node->next_sibling("scene"))
		{
			//for each scene decleration, add an entry to the map
			xml_attribute<> * name_attrib = scene_node->first_attribute("name");
			xml_attribute<> * path_attrib = scene_node->first_attribute("path");

			if(name_attrib && path_attrib)
			{
				//map name to path
				name_to_path_map->insert({name_attrib->value(), path_attrib->value()});
				DEBUG_LOG("Scene {name: " << name_attrib->value() << ", path: " << path_attrib->value() << "}");
			}
		}

		DEBUG_LOG("");

		return name_to_path_map;
	}


	std::unique_ptr<Tag> ProjectLoaderXML::LoadTagDefinitions(const std::string & project_path)
	{
		std::unique_ptr<xml_document<>> doc;
		std::string contents;

		try
		{
			doc = LoadXmlFile(project_path + "/tags.xml", contents);
		}
		catch(const std::exception & e)
		{
			ERROR_LOG(e.what());
			return nullptr;
		}

		DEBUG_LOG("**********  Tag Definitions  **********");

		auto root_tag_node = doc->first_node("tag");
		auto root_tag_name_attrib = (root_tag_node ? root_tag_node->first_attribute("name") : nullptr);

		//every tag hierarchy should be contained in a root_tag with name ""
		std::unique_ptr<Tag> root_tag = std::make_unique<Tag>(root_tag_name_attrib ? root_tag_name_attrib->value() : "");

		for(auto tag_node = doc->first_node("tag"); tag_node; tag_node = tag_node->next_sibling("tag"))
		{
			ParseTag(root_tag->GetSubTags(), tag_node);
		}

		DEBUG_LOG("");

		return root_tag;
	}


	void ProjectLoaderXML::ParseTag(std::vector<Tag> & tags, rapidxml::xml_node<> * tag_node)
	{
		auto name_attrib = tag_node->first_attribute("name");
		const std::string & name = (name_attrib ? name_attrib->value() : "");

		DEBUG_LOG("tag -> name: " << name);

		//add the tags to the tags list
		tags.emplace_back(name);
		auto & new_tag = tags.back();
		auto & sub_list = new_tag.GetSubTags();

		//parse any sub-tags
		for(auto sub_tag_node = tag_node->first_node("tag"); sub_tag_node; sub_tag_node = sub_tag_node->next_sibling("tag"))
		{
			ParseTag(sub_list, sub_tag_node);
		}
	}


	void ProjectLoaderXML::LoadProjectSettings(const std::string & project_path)
	{
		//TODO
	}


	void ProjectLoaderXML::LoadInputSettings(const std::string & project_path)
	{
		//TODO
	}


	std::unique_ptr<Scene> ProjectLoaderXML::LoadScene(const Project & project, const std::string & scene_name)
	{
		std::unique_ptr<xml_document<>> doc;
		std::string contents;
		std::string scene_path;

		// Game ensures scene exists therefore do not need to check here
		auto map = project.GetSceneNamesToPathMap();
		auto pos = map.find(scene_name);
		scene_path = project.GetProjectPath() + "/" + pos->second + ".xml";

		try
		{
			doc = LoadXmlFile(scene_path, contents);
		}
		catch(const std::exception & e)
		{
			ERROR_LOG(e.what());
			return nullptr;
		}

		auto scene_node = doc->first_node("scene");
		auto scene_name_attrib = (scene_node ? scene_node->first_attribute("name") : nullptr);
		///auto aliases_node = scene_node->first_node("aliases");
		auto entities_node = scene_node->first_node("entities");
		auto resources_node = scene_node->first_node("resources");
		///auto cached_prefabs_node = scene_node->first_node("cached_prefabs");

		std::unique_ptr<Scene> scene = std::make_unique<Scene>(scene_name_attrib ? scene_name_attrib->value() : scene_name);

		// map of aliases (lhs = alias, rhs = replacement), only applicable to current file
		std::unordered_map<std::string, std::string> aliases;
		ParseResources(resources_node, aliases, project);
		
		// load the scene's entities
		if(entities_node != nullptr) {
			for(auto entity_node = entities_node->first_node("entity"); entity_node; entity_node = entity_node->next_sibling("entity"))
			{
				// create the entity then move it's pointer to the scene
				auto entity = ParseEntity(entity_node, aliases, project);
				if(entity != nullptr) {
					scene->AddEntity(std::move(entity));
				}
			}
		}

		// remove the temporary prefabs since they were only needed for scene loading
		project.GetPrefabManager().ClearTempPrefabs();

		return scene;
	}


	std::unique_ptr<Entity> ProjectLoaderXML::LoadEntityPrefab(const std::string & prefab_path, const Project & project)
	{
		std::unique_ptr<xml_document<>> doc;
		std::string contents;

		try
		{
			//load the prefab from its xml file
			doc = LoadXmlFile(project.GetProjectPath() + "/Prefabs/" + prefab_path + file_extension, contents);
		}
		catch(const std::exception & e)
		{
			ERROR_LOG(e.what());
			return nullptr;
		}

		auto resources_node = doc->first_node("resources");
		auto entity_node = doc->first_node("entity");

		// map of aliases (lhs = alias, rhs = replacement), only applicable to current file
		std::unordered_map<std::string, std::string> prefab_aliases;
		ParseResources(resources_node, prefab_aliases, project);

		// load the prefab entity as an Entity object
		auto output_entity = ParseEntity(entity_node, prefab_aliases, project);

		// check the entity was successfully loaded and that the name is unique
		if(output_entity && !project.GetPrefabManager().DoesPrefabExist(prefab_path)) {
			return std::move(output_entity);
		}

		return nullptr;
	}


	// returns: Entity object created
	std::unique_ptr<Entity> ProjectLoaderXML::ParseEntity(rapidxml::xml_node<> * entity_node,
			std::unordered_map<std::string, std::string> & aliases, const Project & project)
	{
		auto name_attrib = entity_node->first_attribute("name");
		const std::string & name = (name_attrib ? name_attrib->value() : "");

		auto tag_attrib = entity_node->first_attribute("tag");
		const std::string & tag = (tag_attrib ? tag_attrib->value() : "");

		auto prefab_attrib = entity_node->first_attribute("prefab");
		const std::string & prefab_text = (prefab_attrib ? prefab_attrib->value(): "");

		// if the prefab is an alias, find it's replacement text, else use the file text
		const auto prefab_text_alias_pos = aliases.find(prefab_text);
		const std::string & prefab = prefab_text_alias_pos == aliases.end() ? prefab_text : prefab_text_alias_pos->second;

		// reference to the newly created entity (not yet created)
		std::unique_ptr<Entity> new_entity = nullptr;

		if(prefab == "")
		{
			// if no prefab is specified, then create a new entity object
			new_entity = std::make_unique<Entity>(name, tag, prefab);
		}
		else
		{
			// else, use the existing prefab object as a template
			if(project.GetPrefabManager().DoesPrefabExist(prefab))
			{
				const auto & prefab_object = project.GetPrefabManager().GetPrefab(prefab);
				DEBUG_LOG("Entity " << name << " extends " << prefab_object.GetName() << std::endl);
				new_entity = std::make_unique<Entity>(prefab_object);	// create object from copy of prefab
				new_entity->SetName(name);
				new_entity->SetTag(tag);
			} else {
				DEBUG_LOG("Prefab " << prefab << " does not exist");
			}
		}

		if(new_entity == nullptr) {
			LOG("ERROR: new_entity is null");
			return nullptr;
		}

		// parse the transform component of the new entity
		for(auto component_node = entity_node->first_node("transform"); component_node; component_node = component_node->next_sibling("transform"))
		{
			try
			{
				// TODO - Include controls for settings the orientation
				// NOTE - Should include ability to set 2d rotation (r), 3d euler rotation (rx, ry, rz), 3d orientation (ox, oy, oz, ow)
				float x  { 0.0f };
				float y  { 0.0f };
				float z  { 0.0f };
				float sx { 1.0f };
				float sy { 1.0f };
				float sz { 1.0f };

				auto x_attrib = component_node->first_attribute("x");
				if(x_attrib != nullptr)
					x = std::stof(x_attrib->value());

				auto y_attrib = component_node->first_attribute("y");
				if(y_attrib != nullptr)
					y = std::stof(y_attrib->value());

				auto z_attrib = component_node->first_attribute("z");
				if(z_attrib != nullptr)
					z = std::stof(z_attrib->value());

				auto sx_attrib = component_node->first_attribute("sx");
				if(sx_attrib != nullptr)
					sx = std::stof(sx_attrib->value());

				auto sy_attrib = component_node->first_attribute("sy");
				if(sy_attrib != nullptr)
					sy = std::stof(sy_attrib->value());

				auto sz_attrib = component_node->first_attribute("sz");
				if(sz_attrib != nullptr)
					sz = std::stof(sz_attrib->value());

				new_entity->Translate(x, y, z);
				new_entity->Scale(sx, sy, sz);
			}
			catch(std::exception &)
			{
				ERROR_LOG("Error: Failed to parse transform attribute as float, transform component ignored");
			}
		}

		// parse the sprite renderer components of the new entity
		for(auto component_node = entity_node->first_node("sprite_renderer"); component_node; component_node = component_node->next_sibling("sprite_renderer"))
		{
			// has name & texture attributes
			auto name_attrib	= component_node->first_attribute("name");
			auto texture_attrib = component_node->first_attribute("texture");
			std::string name { (name_attrib ? name_attrib->value() : "") };

			// if texture is an alias, find it's replacement text, else use the file text
			std::string texture_text { (texture_attrib ? texture_attrib->value() : "") };
			const auto texture_text_alias_pos { aliases.find(texture_text) };
			const std::string & texture { texture_text_alias_pos == aliases.end() ? texture_text : texture_text_alias_pos->second };

			const Texture * tex = project.GetResourceManager().GetTexture(texture);
			if(tex != nullptr) {
				new_entity->AddComponent<SpriteRenderer>(name, tex);
			} else {
				ERROR_LOG("Error: Texture " << texture << " has not been loaded");
			}
		}

		// mesh components
		for(auto component_node = entity_node->first_node("mesh_filter"); component_node; component_node = component_node->next_sibling("mesh_filter"))
		{
			// has name & path attributes
			auto name_attrib = component_node->first_attribute("name");
			auto path_attrib = component_node->first_attribute("path");
		//	new_entity.get_components().emplace_back(std::make_unique<MeshFilter>((name_attrib ? name_attrib->value() : ""), (path_attrib ? path_attrib->value() : "")));
		}

		for(auto component_node = entity_node->first_node("mesh_renderer"); component_node; component_node = component_node->next_sibling("mesh_renderer"))
		{
			// has name & path attributes
			auto name_attrib = component_node->first_attribute("name");
			auto path_attrib = component_node->first_attribute("path");
		//	new_entity.get_components().emplace_back(std::make_unique<MeshRenderer>(name_attrib ? name_attrib->value() : ""));
		}

		// parse any sub-entities
		for(auto sub_entity_node = entity_node->first_node("entity"); sub_entity_node; sub_entity_node = sub_entity_node->next_sibling("entity"))
		{
			// create the sub entity then move it's pointer to the new_entity
			auto sub_entity = ParseEntity(sub_entity_node, aliases, project);
			new_entity->AddEntity(std::move(sub_entity));
		}

		return std::move(new_entity);
	}

	void ProjectLoaderXML::ParseResources(rapidxml::xml_node<> * resources_node, std::unordered_map<std::string, std::string> & aliases, const Project & project)
	{
		// parse texture nodes
		for(auto texture_node { resources_node->first_node("texture") }; texture_node; texture_node = texture_node->next_sibling("texture"))
		{
			auto const tex_alias_attrib { texture_node->first_attribute("alias") };
			auto const tex_path_attrib  { texture_node->first_attribute("path") };

			if(tex_path_attrib)
			{
				auto const tex_path { tex_path_attrib->value() };

				// if there is an alias provided, add it to the list of aliases for this file
				if(tex_alias_attrib) {
					auto const tex_alias { tex_alias_attrib->value() };
					aliases.insert({ tex_alias, tex_path });
				}

				project.GetResourceManager().AddTexture(tex_path, "");	// TODO - remove name_ field from texture class
			}
		}

		// parse prefab nodes
		for(auto prefab_node { resources_node->first_node("prefab") }; prefab_node; prefab_node = prefab_node->next_sibling("prefab"))
		{
			auto const prefab_alias_attrib { prefab_node->first_attribute("alias") };
			auto const prefab_path_attrib  { prefab_node->first_attribute("path") };
			auto const prefab_is_cached	   { prefab_node->first_attribute("cached") };
			
			if(prefab_path_attrib)
			{
				auto const prefab_path  { prefab_path_attrib->value() };

				// if there is an alias provided, add it to the list of aliases for this file
				if(prefab_alias_attrib) {
					auto const prefab_alias { prefab_alias_attrib->value() };
					aliases.insert({ prefab_alias, prefab_path });
				}

				// and if so, then load the corresponding prefab file and add it to the map
				// NOTE - currently only check existence of attribute rather than whether it's value is true or false
				if(prefab_is_cached) {
					project.GetPrefabManager().AddCachedPrefab(std::move(LoadEntityPrefab(prefab_path, project)), prefab_path);
				} else {
					project.GetPrefabManager().AddTempPrefab(std::move(LoadEntityPrefab(prefab_path, project)), prefab_path);
				}
			}
		}
	}
}
