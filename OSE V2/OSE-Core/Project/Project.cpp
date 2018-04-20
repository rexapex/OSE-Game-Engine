#include "../../stdafx.h"
#include "Project.h"

namespace ose::project
{
	Project::Project(const std::string & project_path, const ProjectInfo & project_info, const std::map<std::string, std::string> & scene_names_to_path) :
					 project_path_(project_path), project_info_(project_info), scene_names_to_path_(scene_names_to_path)
	{
		this->resource_manager_ = std::make_unique<ResourceManager>(project_path);
	}


	Project::~Project() noexcept
	{
		
	}


	Project::Project(Project && other) noexcept : resource_manager_(std::move(other.resource_manager_))
	{

	}


	Project & Project::operator=(Project && other) noexcept
	{
		resource_manager_ = std::move(other.resource_manager_);
		return *this;
	}
}
