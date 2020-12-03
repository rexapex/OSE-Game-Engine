#pragma once

namespace ose
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

		virtual uptr<ProjectLoader> NewProjectLoader() = 0;
	};
}
