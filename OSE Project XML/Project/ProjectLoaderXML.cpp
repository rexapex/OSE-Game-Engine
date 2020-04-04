#include "pch.h"
#include "ProjectLoaderXML.h"
#include "OSE-Core/File System/FileSystemUtil.h"
#include "OSE-Core/Project/ProjectInfo.h"
#include "OSE-Core/Project/Project.h"
#include "OSE-Core/Project/ProjectSettings.h"
#include "OSE-Core/Game/Tag.h"
#include "OSE-Core/Game/Scene/Scene.h"
#include "OSE-Core/Entity/Entity.h"

#include "OSE-Core/Input/InputSettings.h"
#include "OSE-Core/Input/BooleanInput.h"
#include "OSE-Core/Input/AxisInput.h"
#include "OSE-Core/Input/EInputType.h"

#include "OSE-Core/Resources/Texture/Texture.h"
#include "OSE-Core/Resources/Mesh/Mesh.h"

#include "OSE-Core/Entity/Component/SpriteRenderer.h"
#include "OSE-Core/Entity/Component/TileRenderer.h"
#include "OSE-Core/Entity/Component/MeshRenderer.h"
#include "OSE-Core/Entity/Component/PointLight.h"
#include "OSE-Core/Entity/Component/CustomComponent.h"

#include "OSE-Core/Scripting/ControlSettings.h"

#include "OSE-Core/Resources/Prefab/PrefabManager.h"
#include "OSE-Core/Resources/ResourceManager.h"
#include "OSE-Core/Resources/Custom Data/CustomObject.h"

#include "Dependencies/rapidxml-1.13/rapidxml.hpp"
#include "Dependencies/rapidxml-1.13/rapidxml_print.hpp"

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
			fs::LoadTextFile(path, contents);
		}
		catch(const std::exception & e)
		{
			//error occurred, therefore, return an empty project info stub
			LOG("fs::LoadTextFile ->", e.what());
			throw e;
		}

		//load the xml dom tree
		std::unique_ptr<xml_document<>> doc = std::make_unique<xml_document<>>();
		char * c_str = const_cast<char *>(contents.c_str());
		doc->parse<0>(c_str);

		return doc;
	}
	

	std::unique_ptr<Project> ProjectLoaderXML::LoadProject(const std::string & project_path)
	{
		LOG("Loading Project Directory:", project_path, "\n");

		//first, load the manifest
		std::unique_ptr<ProjectInfo> manifest = LoadProjectManifest(project_path);

		// Then, load the project settings
		ProjectSettings project_settings = LoadProjectSettings(project_path);

		//then, load the scene declerations
		std::unique_ptr<std::map<std::string, std::string>> scene_declerations = LoadSceneDeclerations(project_path);

		//then, load the tag definitions
		std::unique_ptr<Tag> root_tag = LoadTagDefinitions(project_path);

		// Then, load the default input manager
		InputSettings input_settings = LoadInputSettings(project_path);

		// Then, load the persistent control scripts
		ControlSettings control_settings = LoadPersistentControls(project_path);

		//finally, construct a new project instance
		std::unique_ptr<Project> proj = std::make_unique<Project>(project_path, *manifest, project_settings, *scene_declerations, input_settings, control_settings);

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
			LOG_ERROR(e.what());
			return std::make_unique<ProjectInfo>(std::move(ProjectInfo {"UNKNOWN", "UNKNOWN", "UNKNOWN", "UNKNOWN", "UNKNOWN"}));
		}

		DEBUG_LOG("**********  Project Manifest  **********");

		//parse the xml
		//valid project manifest file should contain: name, version, date_created, date_modified
		xml_node<> * name_node = doc->first_node("name");
		std::string name = (name_node ? name_node->value() : "UNKNOWN");
		DEBUG_LOG("name:", name);

		xml_node<> * engine_version_node = doc->first_node("engine_version");
		std::string engine_version = (engine_version_node ? engine_version_node->value() : "UNKNOWN");
		DEBUG_LOG("engine_version:", engine_version);

		xml_node<> * game_version_node = doc->first_node("game_version");
		std::string game_version = (game_version_node ? game_version_node->value() : "UNKNOWN");
		DEBUG_LOG("game_version:", game_version);

		xml_node<> * date_created_node = doc->first_node("date_created");
		std::string date_created = (date_created_node ? date_created_node->value() : "UNKNOWN");
		DEBUG_LOG("date_created:", date_created);

		xml_node<> * date_modified_node = doc->first_node("date_modified");
		std::string date_modified = (date_modified_node ? date_modified_node->value() : "UNKNOWN");
		DEBUG_LOG("date_modified:", date_modified);

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
			LOG_ERROR(e.what());
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
				DEBUG_LOG("Scene { name:", name_attrib->value(), ", path:", path_attrib->value(), " }");
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
			LOG_ERROR(e.what());
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

		DEBUG_LOG("tag -> name:", name);

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


	ProjectSettings ProjectLoaderXML::LoadProjectSettings(const std::string & project_path)
	{
		std::unique_ptr<xml_document<>> doc;
		std::string contents;
		std::string settings_path { project_path + "/settings.xml" };
		ProjectSettings settings;

		try
		{
			doc = LoadXmlFile(settings_path, contents);
		}
		catch(const std::exception & e)
		{
			LOG_ERROR(e.what());
			return settings;
		}

		auto settings_node = doc->first_node("settings");
		if(!settings_node)
			return settings;

		// Process the rendering engine settings
		auto rendering_node = settings_node->first_node("rendering");
		if(rendering_node)
		{
			auto projection_node = rendering_node->first_node("projection");
			if(projection_node)
			{
				// TODO - Ensure 0 < znear < zfar
				try
				{
					auto type_attrib = projection_node->first_attribute("type");
					if(type_attrib != nullptr)
					{
						int type = std::stoi(type_attrib->value());
						if(type == 0 || type == 1)
							settings.rendering_settings_.projection_mode_ = static_cast<EProjectionMode>(type);
						else
							LOG_ERROR("Projection mode must be in range [0, 1]");
					}

					auto znear_attrib = projection_node->first_attribute("znear");
					if(znear_attrib != nullptr)
					{
						float znear = std::stof(znear_attrib->value());
						settings.rendering_settings_.znear_ = znear;
					}

					auto zfar_attrib = projection_node->first_attribute("zfar");
					if(zfar_attrib != nullptr)
					{
						float zfar = std::stof(zfar_attrib->value());
						settings.rendering_settings_.zfar_ = zfar;
					}
				}
				catch(...)
				{
					LOG_ERROR("Failed to parse rendering::projection settings");
				}
			}
		}

		return settings;
	}


	InputSettings ProjectLoaderXML::LoadInputSettings(const std::string & project_path)
	{
		std::unique_ptr<xml_document<>> doc;
		std::string contents;
		std::string input_path { project_path + "/input.xml" };
		InputSettings settings;

		try
		{
			doc = LoadXmlFile(input_path, contents);
		}
		catch(const std::exception & e)
		{
			LOG_ERROR(e.what());
			return settings;
		}

		auto input_node = doc->first_node("input");

		auto parse_input_type = [](std::string const & value) -> EInputType {
			if(value != "")
			{
				try
				{
					return static_cast<EInputType>(std::stoi(value));
				}
				catch(...)
				{
					LOG_ERROR("Failed to parse input value as EInputType from integer");
				}
			}
			return EInputType::NONE;
		};

		// Parse boolean inputs
		for(auto bool_node = input_node->first_node("boolean"); bool_node; bool_node = bool_node->next_sibling("boolean"))
		{
			auto name_attrib = bool_node->first_attribute("name");
			const std::string & name = (name_attrib ? name_attrib->value() : "");
			auto name_iter = settings.boolean_inputs_.find(name);

			auto primary_attrib = bool_node->first_attribute("primary");
			const std::string & primary = (primary_attrib ? primary_attrib->value() : "");

			auto secondary_attrib = bool_node->first_attribute("secondary");
			const std::string & secondary = (secondary_attrib ? secondary_attrib->value() : "");

			// All boolean inputs are required to have a unique name
			if(name != "" && name_iter == settings.boolean_inputs_.end())
			{
				EInputType primary_type { parse_input_type(primary) };
				EInputType secondary_type { parse_input_type(secondary) };
				settings.boolean_inputs_.emplace(name, BooleanInput{ primary_type, secondary_type });
			}
			else
			{
				LOG_ERROR("Failed to parse boolean input, name is a required unique field");
			}
		}

		// Parse axis inputs
		for(auto axis_node = input_node->first_node("axis"); axis_node; axis_node = axis_node->next_sibling("axis"))
		{
			auto name_attrib = axis_node->first_attribute("name");
			const std::string & name = (name_attrib ? name_attrib->value() : "");
			auto name_iter = settings.axis_inputs_.find(name);

			auto pos_primary_attrib = axis_node->first_attribute("pos_primary");
			const std::string & pos_primary = (pos_primary_attrib ? pos_primary_attrib->value() : "");

			auto pos_secondary_attrib = axis_node->first_attribute("pos_secondary");
			const std::string & pos_secondary = (pos_secondary_attrib ? pos_secondary_attrib->value() : "");

			auto neg_primary_attrib = axis_node->first_attribute("neg_primary");
			const std::string & neg_primary = (neg_primary_attrib ? neg_primary_attrib->value() : "");

			auto neg_secondary_attrib = axis_node->first_attribute("neg_secondary");
			const std::string & neg_secondary = (neg_secondary_attrib ? neg_secondary_attrib->value() : "");

			// All axis inputs are required to have a unique name
			if(name != "" && name_iter == settings.axis_inputs_.end())
			{
				EInputType pos_primary_type { parse_input_type(pos_primary) };
				EInputType neg_primary_type { parse_input_type(neg_primary) };
				EInputType pos_secondary_type { parse_input_type(pos_secondary) };
				EInputType neg_secondary_type { parse_input_type(neg_secondary) };
				settings.axis_inputs_.emplace(name, AxisInput{ pos_primary_type, pos_secondary_type, neg_primary_type, neg_secondary_type });
			}
			else
			{
				LOG_ERROR("Failed to parse axis input, name is a required unique field");
			}
		}

		return settings;
	}

	
	// Loads the control scripts which persist through all scenes
	ControlSettings ProjectLoaderXML::LoadPersistentControls(const std::string & project_path)
	{
		std::unique_ptr<xml_document<>> doc;
		std::string contents;
		std::string controls_path { project_path + "/controls.xml" };
		InputSettings settings;

		try
		{
			doc = LoadXmlFile(controls_path, contents);
		}
		catch(const std::exception & e)
		{
			LOG_ERROR(e.what());
			return {};
		}

		auto controls_node = doc->first_node("controls");
		return ParseControls(controls_node);
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
			LOG_ERROR(e.what());
			return nullptr;
		}

		auto scene_node = doc->first_node("scene");
		auto scene_name_attrib = (scene_node ? scene_node->first_attribute("name") : nullptr);
		if(!scene_node)
			return nullptr;
		auto entities_node = scene_node->first_node("entities");
		auto resources_node = scene_node->first_node("resources");
		auto controls_node = scene_node->first_node("controls");
		///auto cached_prefabs_node = scene_node->first_node("cached_prefabs");

		// Load the scene's controls
		ControlSettings control_settings = ParseControls(controls_node);

		// Create the new scene object
		std::unique_ptr<Scene> scene = std::make_unique<Scene>(scene_name_attrib ? scene_name_attrib->value() : scene_name, control_settings);

		// Map of aliases (lhs = alias, rhs = replacement), only applicable to current file
		std::unordered_map<std::string, std::string> aliases;
		ParseResources(resources_node, aliases, project);

		// Load the scene's entities
		if(entities_node != nullptr) {
			for(auto entity_node = entities_node->first_node("entity"); entity_node; entity_node = entity_node->next_sibling("entity"))
			{
				// Parse the xml of the entity and add it to the scene
#				pragma warning(push)
#				pragma warning(disable:26444)
				ParseEntity(scene.get(), entity_node, aliases, project);
#				pragma warning(pop)
			}
		}

		// Remove the temporary prefabs since they were only needed for scene loading
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
			LOG_ERROR(e.what());
			return nullptr;
		}

		auto resources_node = doc->first_node("resources");
		auto entity_node = doc->first_node("entity");

		// map of aliases (lhs = alias, rhs = replacement), only applicable to current file
		std::unordered_map<std::string, std::string> prefab_aliases;
		ParseResources(resources_node, prefab_aliases, project);

		// load the prefab entity as an Entity object
		auto output_entity = ParseEntity(nullptr, entity_node, prefab_aliases, project);

		// check the entity was successfully loaded and that the name is unique
		if(output_entity && !project.GetPrefabManager().DoesPrefabExist(prefab_path)) {
			return std::move(output_entity);
		}

		return nullptr;
	}


	// Parse the XML of an entity
	// If parent != nullptr, the new entity is added to the parent and the return value is nullptr
	// If parent == nullptr, the new entity is returned
	std::unique_ptr<Entity> ProjectLoaderXML::ParseEntity(unowned_ptr<EntityList> parent, rapidxml::xml_node<> * entity_node,
			std::unordered_map<std::string, std::string> & aliases, const Project & project)
	{
		auto name_attrib = entity_node->first_attribute("name");
		const std::string & name = (name_attrib ? name_attrib->value() : "");

		auto tag_attrib = entity_node->first_attribute("tag");
		const std::string & tag = (tag_attrib ? tag_attrib->value() : "");

		auto prefab_attrib = entity_node->first_attribute("prefab");
		const std::string & prefab_text = (prefab_attrib ? prefab_attrib->value(): "");

		// If the prefab is an alias, find it's replacement text, else use the file text
		const auto prefab_text_alias_pos = aliases.find(prefab_text);
		const std::string & prefab = prefab_text_alias_pos == aliases.end() ? prefab_text : prefab_text_alias_pos->second;

		// Pointer to the newly created entity (not yet created)
		unowned_ptr<Entity> new_entity = nullptr;
		std::unique_ptr<Entity> new_entity_ret = nullptr;

		if(prefab == "")
		{
			// If no prefab is specified, then create a new entity object
			if(parent)
				new_entity = parent->AddEntity(name, tag, prefab);
			else
				new_entity_ret = std::make_unique<Entity>(name, tag, prefab), new_entity = new_entity_ret.get();
		}
		else
		{
			// else, use the existing prefab object as a template
			if(project.GetPrefabManager().DoesPrefabExist(prefab))
			{
				const auto & prefab_object = project.GetPrefabManager().GetPrefab(prefab);
				DEBUG_LOG("Entity", name, "extends", prefab_object.GetName(), "\n");
				// Create object from copy of prefab
				if(parent)
					new_entity = parent->AddEntity(prefab_object);
				else
					new_entity_ret = std::make_unique<Entity>(prefab_object), new_entity = new_entity_ret.get();
				new_entity->SetName(name);
				new_entity->SetTag(tag);
			} else {
				DEBUG_LOG("Prefab", prefab, "does not exist");
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
				float rx { 0.0f };
				float ry { 0.0f };
				float rz { 0.0f };

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

				auto rx_attrib = component_node->first_attribute("rx");
				if(rx_attrib != nullptr)
					rx = std::stof(rx_attrib->value());

				auto ry_attrib = component_node->first_attribute("ry");
				if(ry_attrib != nullptr)
					ry = std::stof(ry_attrib->value());

				auto rz_attrib = component_node->first_attribute("rz");
				if(rz_attrib != nullptr)
					rz = std::stof(rz_attrib->value());

				new_entity->Translate(x, y, z);
				new_entity->Scale(sx, sy, sz);
				new_entity->RotateDeg(rx, ry, rz);
			}
			catch(...)
			{
				LOG_ERROR("Failed to parse transform attribute as float, transform component ignored");
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
				LOG_ERROR("Texture", texture, "has not been loaded");
			}
		}

		// Parse the tile renderer components of the new entity
		for(auto component_node = entity_node->first_node("tile_renderer"); component_node; component_node = component_node->next_sibling("tile_renderer"))
		{
			// Has name, texture & tilemap attributes
			auto name_attrib	= component_node->first_attribute("name");
			auto texture_attrib = component_node->first_attribute("texture");
			auto tilemap_attrib = component_node->first_attribute("tilemap");
			std::string name { (name_attrib ? name_attrib->value() : "") };

			// Optionally has num_cols, num_rows, num_tiles, spacing_x & spacing_y attributes
			auto num_cols_attrib  = component_node->first_attribute("num_cols");
			auto num_rows_attrib  = component_node->first_attribute("num_rows");
			auto num_tiles_attrib = component_node->first_attribute("num_tiles");
			auto spacing_x_attrib = component_node->first_attribute("spacing_x");
			auto spacing_y_attrib = component_node->first_attribute("spacing_y");
			int32_t num_cols{ 0 };
			int32_t num_rows{ 0 };
			int32_t num_tiles{ 0 };
			float spacing_x{ 1.0f };
			float spacing_y{ 1.0f };

			try
			{
				if(num_cols_attrib != nullptr)
					num_cols = std::stoi(num_cols_attrib->value());
				if(num_rows_attrib != nullptr)
					num_rows = std::stoi(num_rows_attrib->value());
				if(num_tiles_attrib != nullptr)
					num_tiles = std::stoi(num_tiles_attrib->value());
				if(spacing_x_attrib != nullptr)
					spacing_x = std::stof(spacing_x_attrib->value());
				if(spacing_y_attrib != nullptr)
					spacing_y = std::stof(spacing_y_attrib->value());
			}
			catch(...)
			{
				LOG_ERROR("Failed to parse num_cols/num_rows/num_tiles/spacing_x/spacing_y attribute(s) as integer");
			}

			// If texture is an alias, find it's replacement text, else use the file text
			std::string texture_text { (texture_attrib ? texture_attrib->value() : "") };
			const auto texture_text_alias_pos { aliases.find(texture_text) };
			const std::string & texture { texture_text_alias_pos == aliases.end() ? texture_text : texture_text_alias_pos->second };

			// If tilemap is an alias, find it's replacement text, else use the file text
			std::string tilemap_text { (tilemap_attrib ? tilemap_attrib->value() : "") };
			const auto tilemap_text_alias_pos { aliases.find(tilemap_text) };
			const std::string & tilemap { tilemap_text_alias_pos == aliases.end() ? tilemap_text : tilemap_text_alias_pos->second };

			// Add the component to the entity
			const Tilemap * tmap = project.GetResourceManager().GetTilemap(tilemap);
			const Texture * tex = project.GetResourceManager().GetTexture(texture);
			if(tex != nullptr && tmap != nullptr) {
				new_entity->AddComponent<TileRenderer>(name, tex, tmap, num_cols, num_rows, num_tiles, spacing_x, spacing_y);
			} else {
				if(tex == nullptr) {
					LOG_ERROR("Texture", texture, "has not been loaded");
				}
				if(tmap == nullptr) {
					LOG_ERROR("Tilemap", tilemap, "has not been loaded");
				}
			}
		}

		// parse the mesh renderer components of the new entity
		for(auto component_node = entity_node->first_node("mesh_renderer"); component_node; component_node = component_node->next_sibling("mesh_renderer"))
		{
			// Has name & mesh attributes
			auto name_attrib = component_node->first_attribute("name");
			auto mesh_attrib = component_node->first_attribute("mesh");
			std::string name { (name_attrib ? name_attrib->value() : "") };

			// Optionally has material attribute
			auto material_attrib = component_node->first_attribute("material");

			// If mesh is an alias, find its replacement text, else use the file text
			std::string mesh_text { (mesh_attrib ? mesh_attrib->value() : "") };
			const auto mesh_text_alias_pos { aliases.find(mesh_text) };
			const std::string & mesh_path { mesh_text_alias_pos == aliases.end() ? mesh_text : mesh_text_alias_pos->second };
			
			// If material is an alias, find its replacement text, else us the file text
			std::string material_text { (material_attrib ? material_attrib->value() : "") };
			const auto material_text_alias_pos { aliases.find(material_text) };
			const std::string & material_path { material_text_alias_pos == aliases.end() ? material_text : material_text_alias_pos->second };

			const Mesh * mesh = project.GetResourceManager().GetMesh(mesh_path);
			const Material * material = project.GetResourceManager().GetMaterial(material_path);
			if(mesh != nullptr) {
				new_entity->AddComponent<MeshRenderer>(name, mesh, material);
			} else {
				LOG_ERROR("Mesh", mesh_path, "has not been loaded");
			}
		}

		// parse the point light components of the entity
		for(auto component_node = entity_node->first_node("point_light"); component_node; component_node = component_node->next_sibling("point_light"))
		{
			// Has attribute
			auto name_attrib = component_node->first_attribute("name");
			std::string name { (name_attrib ? name_attrib->value() : "") };

			// Optionally has color attributes
			glm::vec3 color { 0.0f, 0.0f, 0.0f };
			auto color_r_attrib = component_node->first_attribute("color_r");
			auto color_g_attrib = component_node->first_attribute("color_g");
			auto color_b_attrib = component_node->first_attribute("color_b");
			try
			{
				if(color_r_attrib != nullptr)
					color.r = std::stof(color_r_attrib->value());
				if(color_g_attrib != nullptr)
					color.g = std::stof(color_g_attrib->value());
				if(color_b_attrib != nullptr)
					color.b = std::stof(color_b_attrib->value());
			}
			catch(...)
			{
				LOG_ERROR("Failed to parse point light", name, "color data");
			}

			// Optionally has intensity attribute
			float intensity { 1.0f };
			auto intensity_attrib = component_node->first_attribute("intensity");
			try
			{
				if(intensity_attrib != nullptr)
					intensity = std::stof(intensity_attrib->value());
			}
			catch(...)
			{
				LOG_ERROR("Failed to parse point light", name, "intensity data");
			}

			new_entity->AddComponent<PointLight>(name, color, intensity);
		}

		// parse the custom component components of the entity
		for(auto component_node = entity_node->first_node("custom"); component_node; component_node = component_node->next_sibling("custom"))
		{
			// has name and component attributes
			auto name_attrib	= component_node->first_attribute("name");
			auto component_attrib = component_node->first_attribute("component");
			std::string name { (name_attrib ? name_attrib->value() : "") };
			std::string component { (component_attrib ? component_attrib->value() : "") };
			new_entity->AddComponent<CustomComponent>(name, component);
		}

		// parse any sub-entities
		for(auto sub_entity_node = entity_node->first_node("entity"); sub_entity_node; sub_entity_node = sub_entity_node->next_sibling("entity"))
		{
			// parse the sub entity's xml
#			pragma warning(push)
#			pragma warning(disable:26444)
			ParseEntity(new_entity, sub_entity_node, aliases, project);
#			pragma warning(pop)
		}

		return new_entity_ret;
	}

	void ProjectLoaderXML::ParseResources(rapidxml::xml_node<> * resources_node, std::unordered_map<std::string, std::string> & aliases, const Project & project)
	{
		if(!resources_node)
			return;

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

		// parse tilemap nodes
		for(auto tilemap_node { resources_node->first_node("tilemap") }; tilemap_node; tilemap_node = tilemap_node->next_sibling("tilemap"))
		{
			auto const alias_attrib { tilemap_node->first_attribute("alias") };
			auto const path_attrib  { tilemap_node->first_attribute("path") };

			if(path_attrib)
			{
				auto const path { path_attrib->value() };

				// if there is an alias provided, add it to the list of aliases for this file
				if(alias_attrib) {
					auto const alias { alias_attrib->value() };
					aliases.insert({ alias, path });
				}

				project.GetResourceManager().AddTilemap(path, "");	// TODO - remove name_ field from texture class
			}
		}

		// parse mesh nodes
		for(auto mesh_node { resources_node->first_node("mesh") }; mesh_node; mesh_node = mesh_node->next_sibling("mesh"))
		{
			auto const alias_attrib { mesh_node->first_attribute("alias") };
			auto const path_attrib  { mesh_node->first_attribute("path") };

			if(path_attrib)
			{
				auto const path { path_attrib->value() };

				// if there is an alias provided, add it to the list of aliases for this file
				if(alias_attrib) {
					auto const alias { alias_attrib->value() };
					aliases.insert({ alias, path });
				}

				project.GetResourceManager().AddMesh(path, "");	// TODO - remove name_ field from mesh class
			}
		}

		// parse material nodes
		for(auto material_node { resources_node->first_node("material") }; material_node; material_node = material_node->next_sibling("material"))
		{
			auto const alias_attrib { material_node->first_attribute("alias") };
			auto const path_attrib  { material_node->first_attribute("path") };

			if(path_attrib)
			{
				auto const path { path_attrib->value() };

				// if there is an alias provided, add it to the list of aliases for this file
				if(alias_attrib) {
					auto const alias { alias_attrib->value() };
					aliases.insert({ alias, path });
				}

				project.GetResourceManager().AddMaterial(path, "");	// TODO - remove name_ field from material class
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

	ControlSettings ProjectLoaderXML::ParseControls(rapidxml::xml_node<> * controls_node)
	{
		ControlSettings settings;
		if(controls_node)
		{
			for(auto control_node = controls_node->first_node("control"); control_node; control_node = control_node->next_sibling("control"))
			{
				auto type_attrib = control_node->first_attribute("type");
				auto deferred_attrib = control_node->first_attribute("deferred");
				std::string type { (type_attrib ? type_attrib->value() : "") };
				bool deferred { deferred_attrib ? deferred_attrib->value() == "true" : false };
				if(deferred)
					settings.deferred_controls_.emplace_back(type);
				else
					settings.controls_.emplace_back(type);
			}
		}
		return settings;
	}
	

	// Load a custom data file into a custom object
	std::unique_ptr<CustomObject> ProjectLoaderXML::LoadCustomDataFile(const std::string & path)
	{
		std::unique_ptr<xml_document<>> doc;
		std::string contents;

		try
		{
			doc = LoadXmlFile(path, contents);
		}
		catch(const std::exception & e)
		{
			LOG_ERROR(e.what());
			return nullptr;
		}

		DEBUG_LOG("**********  Custom Data File  **********");

		// Parse the xml
		xml_node<> * root_obj = doc->first_node("object");
		try
		{
			return ParseCustomObject(root_obj);
		}
		catch(std::exception & e)
		{
			LOG_ERROR(e.what());
		}
		return nullptr;
	}
	
	// Parse a custom object node
	std::unique_ptr<CustomObject> ProjectLoaderXML::ParseCustomObject(rapidxml::xml_node<> * obj_node)
	{
		if(obj_node == nullptr)
			return nullptr;

		auto obj = std::make_unique<CustomObject>();

		// Get the key and value pair from an xml node
		auto get_keyval_pair = [](CustomObject & parent, rapidxml::xml_node<> * node, std::string const & default_value) -> std::pair<std::string, std::string> {
			auto name_attrib = node->first_attribute("name");
			std::string name { (name_attrib ? name_attrib->value() : "") };

			if(name != "" && parent.data_.find(name) == parent.data_.end())
			{
				std::string value_str = (node ? node->value() : default_value);
				return std::make_pair(name, value_str);
			}
			else
			{
				LOG_ERROR("Custom object field name (", name, ") is not given or already in use");
				throw std::exception("Failed to parse custom object");
			}
		};

		// Parse integer nodes
		for(auto int_node { obj_node->first_node("int") }; int_node; int_node = int_node->next_sibling("int"))
		{
			auto pair { get_keyval_pair(*obj, int_node, "0") };
			try
			{
				int64_t value = std::stoll(pair.second);
				obj->data_[pair.first] = value;
			}
			catch(...)
			{
				LOG_ERROR("Failed to parse INT data in custom object");
				return nullptr;
			}
		}

		// Parse float nodes
		for(auto float_node { obj_node->first_node("float") }; float_node; float_node = float_node->next_sibling("float"))
		{
			auto pair { get_keyval_pair(*obj, float_node, "0.0") };
			try
			{
				double value = std::stod(pair.second);
				obj->data_[pair.first] = value;
			}
			catch(...)
			{
				LOG_ERROR("Failed to parse FLOAT data in custom object");
				return nullptr;
			}
		}

		// Parse boolean nodes
		for(auto bool_node { obj_node->first_node("bool") }; bool_node; bool_node = bool_node->next_sibling("float"))
		{
			auto pair { get_keyval_pair(*obj, bool_node, "0") };
			bool value = pair.second == "true" || pair.second == "TRUE" || pair.second == "1" ? true : false;
			obj->data_[pair.first] = value;
		}

		// Parse string nodes
		for(auto string_node { obj_node->first_node("string") }; string_node; string_node = string_node->next_sibling("string"))
		{
			auto pair { get_keyval_pair(*obj, string_node, "") };
			obj->data_[pair.first] = pair.second;
		}

		// Parse nested custom object nodes
		for(auto child_obj_node { obj_node->first_node("object") }; child_obj_node; child_obj_node = child_obj_node->next_sibling("object"))
		{
			auto pair { get_keyval_pair(*obj, child_obj_node, "") };
			auto child_obj { ParseCustomObject(child_obj_node) };
			if(child_obj)
				obj->data_[pair.first] = std::move(child_obj);
		}

		// Parse array of integer nodes
		for(auto ints_node { obj_node->first_node("ints") }; ints_node; ints_node = ints_node->next_sibling("ints"))
		{
			auto pair { get_keyval_pair(*obj, ints_node, "") };
			try
			{
				std::vector<int64_t> value;
				std::stringstream ss(pair.second);
				std::string int_str;
				while(ss.good())
				{
					std::getline(ss, int_str, ',');
					value.push_back(std::stoll(int_str));
				}
				obj->data_[pair.first] = value;
			}
			catch(...)
			{
				LOG_ERROR("Failed to parse INT array data in custom object");
				return nullptr;
			}
		}

		// Parse array of float nodes
		for(auto floats_node { obj_node->first_node("floats") }; floats_node; floats_node = floats_node->next_sibling("floats"))
		{
			auto pair { get_keyval_pair(*obj, floats_node, "") };
			try
			{
				std::vector<double> value;
				std::stringstream ss(pair.second);
				std::string float_str;
				while(ss.good())
				{
					std::getline(ss, float_str, ',');
					value.push_back(std::stod(float_str));
				}
				obj->data_[pair.first] = value;
			}
			catch(...)
			{
				LOG_ERROR("Failed to parse FLOAT array data in custom object");
				return nullptr;
			}
		}

		// Parse array of bool nodes
		for(auto bools_node { obj_node->first_node("bools") }; bools_node; bools_node = bools_node->next_sibling("bools"))
		{
			auto pair { get_keyval_pair(*obj, bools_node, "") };
			std::vector<bool> value;
			std::stringstream ss(pair.second);	// TODO - Trim pair.second so whitespace is ignored
			std::string bool_str;
			while(ss.good())
			{
				std::getline(ss, bool_str, ',');
				value.push_back(bool_str == "true" || bool_str == "TRUE" || bool_str == "1" ? true : false);
			}
			obj->data_[pair.first] = value;
		}

		// Parse array of string nodes
		for(auto strings_node { obj_node->first_node("strings") }; strings_node; strings_node = strings_node->next_sibling("strings"))
		{
			auto pair { get_keyval_pair(*obj, strings_node, "") };
			std::vector<std::string> value;
			for(auto string_node { strings_node->first_node("string") }; string_node; string_node = string_node->next_sibling("string"))
			{
				auto string_value = string_node ? string_node->value() : "";
				value.push_back(string_value);
			}
			obj->data_[pair.first] = value;
		}

		// Parse array of object nodes
		for(auto objects_node { obj_node->first_node("objects") }; objects_node; objects_node = objects_node->next_sibling("objects"))
		{
			auto pair { get_keyval_pair(*obj, objects_node, "") };
			std::vector<std::unique_ptr<CustomObject>> value;
			for(auto child_object_node { objects_node->first_node("object") }; child_object_node; child_object_node = child_object_node->next_sibling("object"))
			{
				auto obj = ParseCustomObject(child_object_node);
				if(obj)
					value.push_back(std::move(obj));
			}
			obj->data_[pair.first] = std::move(value);
		}

		return obj;
	}

	void ProjectLoaderXML::SaveCustomDataFile(const std::string & path, CustomObject const & object)
	{
		std::unique_ptr<xml_document<>> doc { std::make_unique<xml_document<>>() };
		SaveCustomDataObject(*doc, object);
		std::stringstream ss;
		ss << *doc;
		fs::WriteTextFile(path, ss.str());
	}

	void ProjectLoaderXML::SaveCustomDataObject(xml_document<> & doc, CustomObject const & object, xml_node<> * parent, std::string name)
	{
		xml_node<> * obj_node = doc.allocate_node(node_element, "object");
		
		// Add a name attribute to the object if one is given
		if(name != "")
		{
			char * node_name = doc.allocate_string(name.c_str());
			auto name_attr = doc.allocate_attribute("name", node_name);
			obj_node->append_attribute(name_attr);
		}

		// Add the object to the xml doc
		if(parent)
			parent->append_node(obj_node);
		else
			doc.append_node(obj_node);

		// Process the data contained in the object
		for(auto & pair : object.data_)
		{
			auto node_data = std::visit([this, &doc, obj_node, &pair](auto && arg) -> std::pair<std::string, std::string> {
				using T = std::decay_t<decltype(arg)>;
				if constexpr(std::is_same_v<T, int64_t>)
				{
					return std::make_pair("int", std::to_string(arg));
				}
				else if constexpr(std::is_same_v<T, double>)
				{
					return std::make_pair("float", std::to_string(arg));
				}
				else if constexpr(std::is_same_v<T, bool>)
				{
					return std::make_pair("bool", arg ? "true" : "false");
				}
				else if constexpr(std::is_same_v<T, std::string>)
				{
					return std::make_pair("string", arg);
				}
				else if constexpr(std::is_same_v<T, std::unique_ptr<CustomObject>>)
				{
					SaveCustomDataObject(doc, *arg, obj_node, pair.first);
				}
				else if constexpr(std::is_same_v<T, std::vector<int64_t>>)
				{
					std::string s;
					for(auto i : arg)
					{
						s += std::to_string(i) + ",";
					}
					if(s.size() > 0)
						s.erase(s.end()-1);
					return std::make_pair("ints", s);
				}
				else if constexpr(std::is_same_v<T, std::vector<double>>)
				{
					std::string s;
					for(auto d : arg)
					{
						s += std::to_string(d) + ",";
					}
					if(s.size() > 0)
						s.erase(s.end()-1);
					return std::make_pair("floats", s);
				}
				else if constexpr(std::is_same_v<T, std::vector<bool>>)
				{
					std::string s;
					for(auto b : arg)
					{
						s += (b ? std::string("true") : std::string("false")) + ",";
					}
					if(s.size() > 0)
						s.erase(s.end()-1);
					return std::make_pair("bools", s);
				}
				else if constexpr(std::is_same_v<T, std::vector<std::string>>)
				{
					char * node_name = doc.allocate_string(pair.first.c_str());
					auto node = doc.allocate_node(node_element, "strings");
					auto name_attr = doc.allocate_attribute("name", node_name);
					node->append_attribute(name_attr);
					for(auto const & s : arg)
					{
						char * elem_node_value = doc.allocate_string(s.c_str());
						auto elem_node = doc.allocate_node(node_element, "string", elem_node_value);
						node->append_node(elem_node);
					}
					obj_node->append_node(node);
				}
				else if constexpr(std::is_same_v<T, std::vector<std::unique_ptr<CustomObject>>>)
				{
					char * node_name = doc.allocate_string(pair.first.c_str());
					auto node = doc.allocate_node(node_element, "objects");
					auto name_attr = doc.allocate_attribute("name", node_name);
					node->append_attribute(name_attr);
					for(auto const & obj : arg)
					{
						SaveCustomDataObject(doc, *obj, node);
					}
					obj_node->append_node(node);
				}
				return std::make_pair("", "");
			}, pair.second);

			if(node_data.first == "int" || node_data.first == "float" || node_data.first == "bool" || node_data.first == "string"
				|| node_data.first == "ints" || node_data.first == "floats" || node_data.first == "bools")
			{
				char * node_type = doc.allocate_string(node_data.first.c_str());
				char * node_value = doc.allocate_string(node_data.second.c_str());
				char * node_name = doc.allocate_string(pair.first.c_str());
				auto node = doc.allocate_node(node_element, node_type, node_value);
				auto name_attr = doc.allocate_attribute("name", node_name);
				node->append_attribute(name_attr);
				obj_node->append_node(node);
			}
		}
	}
}
