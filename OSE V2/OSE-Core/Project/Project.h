#pragma once

#include "ProjectInfo.h"
#include "OSE-Core/Input/InputSettings.h"
#include "OSE-Core/Scripting/ControlSettings.h"

namespace ose
{
	class ResourceManager;
	class PrefabManager;

	// A representation of a game project as a whole
	// Contains information about the path, the resources & the scenes available
	// Also contains the default settings for the input and the engines
	// Can be used to edit the project during engine runtime
	class Project
	{
	public:
		Project(const std::string & project_path, const ProjectInfo & project_info, const std::map<std::string, std::string> & scene_names_to_path,
			InputSettings const & input_settings, ControlSettings const & control_settings);
		virtual ~Project() noexcept;
		Project(Project && other) noexcept;
		Project & operator=(Project && other) noexcept;

		const ProjectInfo & GetProjectInfo() const { return this->project_info_; }
		const std::string & GetProjectPath() const { return this->project_path_; }
		const std::string & GetProjectFileFormat() const { return this->project_file_format_; }
		const std::map<std::string, std::string> & GetSceneNamesToPathMap() const { return this->scene_names_to_path_; }
		ResourceManager & GetResourceManager() const { return *resource_manager_; }
		PrefabManager & GetPrefabManager() const { return *prefab_manager_; }

		InputSettings const & GetInputSettings() const { return input_settings_; }
		ControlSettings const & GetControlSettings() const { return control_settings_; }

		// Create gpu resources for each loaded resource object
		void CreateGpuResources();

	private:
		// absolute path of the root directory of the project
		std::string project_path_;

		// the format of all project files (used to determine which project loader impl to use)
		std::string project_file_format_;
		
		// project info (name, version, date_created, date_modified, etc...)
		ProjectInfo project_info_;

		// resource manager
		std::unique_ptr<ResourceManager> resource_manager_;

		// prefab manager
		std::unique_ptr<PrefabManager> prefab_manager_;

		// scene list (maps name to path ?)
		std::map<std::string, std::string> scene_names_to_path_;

		// The default input settings configured in the project
		InputSettings input_settings_;

		// The control scripts which persist across scenes
		ControlSettings control_settings_;

		// engine settings
		// rendering/physics settings
		// input settings
	};
}

