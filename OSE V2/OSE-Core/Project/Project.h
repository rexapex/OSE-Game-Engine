#pragma once

#include "ProjectInfo.h"
#include "ProjectSettings.h"
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
		Project(std::string const & project_path, ProjectInfo const & project_info, ProjectSettings const & project_settings,
			std::map<std::string, std::string> const & scene_names_to_path,
			InputSettings const & input_settings, ControlSettings const & control_settings);
		virtual ~Project() noexcept;
		Project(Project && other) noexcept;
		Project & operator=(Project && other) noexcept;

		ProjectInfo const & GetProjectInfo() const { return project_info_; }
		std::string const & GetProjectPath() const { return project_path_; }
		std::string const & GetProjectFileFormat() const { return project_file_format_; }
		std::map<std::string, std::string> const & GetSceneNamesToPathMap() const { return scene_names_to_path_; }
		ResourceManager & GetResourceManager() const { return *resource_manager_; }
		PrefabManager & GetPrefabManager() const { return *prefab_manager_; }

		ProjectSettings const & GetProjectSettings() const { return project_settings_; }
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

		// project settings (settings for each engine)
		ProjectSettings project_settings_;

		// resource manager
		uptr<ResourceManager> resource_manager_;

		// prefab manager
		uptr<PrefabManager> prefab_manager_;

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

