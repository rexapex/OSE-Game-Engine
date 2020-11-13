#include "stdafx.h"
#include "Project.h"
#include "OSE-Core/Resources/ResourceManager.h"
#include "OSE-Core/Resources/Prefab/PrefabManager.h"

namespace ose
{
	Project::Project(std::string const & project_path, ProjectInfo const & project_info, ProjectSettings const & project_settings,
		std::map<std::string, std::string> const & scene_names_to_path,
		InputSettings const & input_settings, ControlSettings const & control_settings)
		: project_path_(project_path), project_info_(project_info), project_settings_(project_settings),
		scene_names_to_path_(scene_names_to_path),
		input_settings_(input_settings), control_settings_(control_settings)
	{
		resource_manager_ = ose::make_unique<ResourceManager>(project_path);
		prefab_manager_ = ose::make_unique<PrefabManager>();
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
		prefab_manager_ = std::move(other.prefab_manager_);
		return *this;
	}

	// Create gpu resources for each loaded resource object
	void Project::CreateGpuResources()
	{
		resource_manager_->CreateTextures();
		resource_manager_->CreateShaderProgs();
	}
}
