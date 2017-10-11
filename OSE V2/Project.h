#pragma once

#include "ProjectInfo.h"
#include "ResourceManager.h"

namespace origami_sheep_engine
{
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

		const std::string & get_project_path() const { return this->project_path_; }
		const std::map<std::string, std::string> & get_scene_names_to_path_map() const { return this->scene_names_to_path_; }

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

