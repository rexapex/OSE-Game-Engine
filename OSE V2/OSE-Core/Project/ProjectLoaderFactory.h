#pragma once

namespace ose::project
{
	class ProjectLoader;

	class ProjectLoaderFactory
	{
	public:
		constexpr ProjectLoaderFactory() {}
		virtual ~ProjectLoaderFactory() {}
		ProjectLoaderFactory(ProjectLoaderFactory &) = delete;
		ProjectLoaderFactory & operator=(ProjectLoaderFactory &) = delete;
		ProjectLoaderFactory(ProjectLoaderFactory &&) = default;
		ProjectLoaderFactory & operator=(ProjectLoaderFactory &&) = default;

		virtual std::unique_ptr<ProjectLoader> NewProjectLoader() = 0;
	};
}
