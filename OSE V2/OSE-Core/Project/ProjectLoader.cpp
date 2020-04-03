#include "stdafx.h"
#include "ProjectLoader.h"
#include "OSE-Core/Resources/FileSystemUtil.h"

namespace ose
{
	ProjectLoader::ProjectLoader()
	{
	}

	ProjectLoader::~ProjectLoader() noexcept
	{
	}

	std::string ProjectLoader::GetProjectFileFormat(const std::string & project_path)
	{
		std::string text;
		fs::LoadTextFile(project_path + "/PROJECT_FORMAT", text);
		return text;
	}
}