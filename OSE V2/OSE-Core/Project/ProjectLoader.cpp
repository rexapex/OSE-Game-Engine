#include "stdafx.h"
#include "ProjectLoader.h"
#include "OSE-Core/File System/FileSystemUtil.h"

namespace ose
{
	ProjectLoader::ProjectLoader()
	{
	}

	ProjectLoader::~ProjectLoader() noexcept
	{
	}

	std::string ProjectLoader::GetProjectFileFormat(std::string const & project_path)
	{
		std::string text;
		fs::LoadTextFile(project_path + "/PROJECT_FORMAT", text);
		return text;
	}
}