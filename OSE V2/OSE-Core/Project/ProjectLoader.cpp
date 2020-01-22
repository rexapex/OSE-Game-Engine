#include "stdafx.h"
#include "ProjectLoader.h"
#include "OSE-Core/Resources/FileHandlingUtil.h"

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
		FileHandlingUtil::LoadTextFile(project_path + "/PROJECT_FORMAT", text);
		return text;
	}
}