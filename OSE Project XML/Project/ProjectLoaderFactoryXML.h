#pragma once
#include "OSE-Core/Project/ProjectLoaderFactory.h"

namespace ose
{
	class ProjectLoader;
}

namespace ose::project
{
	class ProjectLoaderFactoryXML : public ProjectLoaderFactory
	{
	public:
		constexpr ProjectLoaderFactoryXML() {}
		virtual ~ProjectLoaderFactoryXML() {}
		ProjectLoaderFactoryXML(ProjectLoaderFactoryXML &) = delete;
		ProjectLoaderFactoryXML & operator=(ProjectLoaderFactoryXML &) = delete;
		ProjectLoaderFactoryXML(ProjectLoaderFactoryXML &&) = default;
		ProjectLoaderFactoryXML & operator=(ProjectLoaderFactoryXML &&) = default;

		virtual uptr<ProjectLoader> NewProjectLoader();
	};
}
