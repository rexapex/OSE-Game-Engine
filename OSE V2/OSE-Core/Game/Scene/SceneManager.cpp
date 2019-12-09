#include "stdafx.h"
#include "SceneManager.h"
#include "OSE-Core/Project/Project.h"
#include "OSE-Core/Project/ProjectLoader.h"
#include "OSE-Core/EngineReferences.h"

namespace ose::game
{
	SceneManager::SceneManager()
	{
		this->project_loader_ = std::move(ProjectLoaderFactories[0]->NewProjectLoader());
	}

	SceneManager::~SceneManager() noexcept
	{

	}

	// loads the project specified (does not load any scenes)
	// throws std::exception if the project could not be loaded
	// throws std::invalid_argument if the project FILE_FORMAT decleration file does not exist
	void SceneManager::LoadProject(const std::string & proj_name)
	{
		// before project can be loaded, the file format of the project files must de determined
		std::string proj_file_format = ose::project::ProjectLoader::GetProjectFileFormat(proj_name);

		if(proj_file_format == "XML") {
			this->project_ = this->project_loader_->LoadProject(proj_name);

			if(this->project_ == nullptr) {
				throw std::exception("Error: Could not load Project");
			}
		} else {
			throw std::exception("Error: Unknown project file type");
		}
	}
}
