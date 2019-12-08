#include "pch.h"
#include "ProjectLoaderFactoryXML.h"
#include "ProjectLoaderXML.h"

namespace ose::project
{
	std::unique_ptr<ProjectLoader> ProjectLoaderFactoryXML::NewProjectLoader()
	{
		return std::make_unique<ProjectLoaderXML>();
	}
}
