#include "pch.h"
#include "ProjectLoaderFactoryXML.h"
#include "ProjectLoaderXML.h"

namespace ose::project
{
	uptr<ProjectLoader> ProjectLoaderFactoryXML::NewProjectLoader()
	{
		return ose::make_unique<ProjectLoaderXML>();
	}
}
