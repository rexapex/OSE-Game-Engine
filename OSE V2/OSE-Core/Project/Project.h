#pragma once

#include "ProjectInfo.h"
#include "../Resources/ResourceManager.h"

namespace ose::project
{
	using namespace resources;

	//A representation of a game project as a whole
	//Contains information about the path, the resources & the scenes available
	//Also contains the default settings for the input and the engines
	//Can be used to edit the project during engine runtime
	class Project
	{
	public:
		Project(const std::string & project_path, const ProjectInfo & project_info, const std::map<std::string, std::string> & scene_names_to_path);
		~Project() noexcept;
		Project(Project && other) noexcept;
		Project & operator=(Project && other) noexcept;

		const ProjectInfo & get_project_info() const { return this->project_info_; }
		const std::string & get_project_path() const { return this->project_path_; }
		const std::map<std::string, std::string> & get_scene_names_to_path_map() const { return this->scene_names_to_path_; }
		ResourceManager * const get_resource_manager() const { return resource_manager_.get(); }

	private:
		std::string project_path_;
		
		//project info (name, version, date_created, date_modified, etc...)
		ProjectInfo project_info_;

		//resource manager
		std::unique_ptr<ResourceManager> resource_manager_;

		//scene list (maps name to path ?)
		std::map<std::string, std::string> scene_names_to_path_;

		//engine settings
		//rendering/physics settings
		//input settings
	};
}

