#include "stdafx.h"
#include "ProjectLoader.h"

namespace ose::project
{
	ProjectLoader::ProjectLoader()
	{
	}

	ProjectLoader::~ProjectLoader() noexcept
	{
	}

	std::string ProjectLoader::GetProjectFileFormat(const std::string & project_name)
	{
		std::string home_dir;
		FileHandlingUtil::GetHomeDirectory(home_dir);

		//TODO - FIND DOCUMENT DIRECTORY FOR MAC & LINUX - DONE - NEEDS TESTING
		//TODO - CREATE DIRECTORIES IF THEY DON'T EXIST  - DONE - NEEDS TESTING
		std::string project_path = home_dir + "/Origami_Sheep_Engine/Projects/" + project_name;
		FileHandlingUtil::CreateDirs(project_path);

		std::string text;
		FileHandlingUtil::LoadTextFile(project_path + "/PROJECT_FORMAT", text);

		return text;
	}
}