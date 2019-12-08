#pragma once
#include "OSE-Core/Project/ProjectLoaderFactory.h"

namespace ose::project
{
	class ProjectLoader;

	class ProjectLoaderFactoryXML : public ProjectLoaderFactory
	{
	public:
		constexpr ProjectLoaderFactoryXML() {}
		virtual ~ProjectLoaderFactoryXML() {}
		ProjectLoaderFactoryXML(ProjectLoaderFactoryXML &) = delete;
		ProjectLoaderFactoryXML & operator=(ProjectLoaderFactoryXML &) = delete;
		ProjectLoaderFactoryXML(ProjectLoaderFactoryXML &&) = default;
		ProjectLoaderFactoryXML & operator=(ProjectLoaderFactoryXML &&) = default;

		virtual std::unique_ptr<ProjectLoader> NewProjectLoader();
	};
}
