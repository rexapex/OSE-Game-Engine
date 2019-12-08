#pragma once

namespace ose::resources
{
	class ModelLoader
	{
	public:
		ModelLoader(const std::string & project_path);
		~ModelLoader();
		//ModelLoader is NOT copyable
		ModelLoader(ModelLoader &) = delete;
		ModelLoader & operator=(ModelLoader &) = delete;
		//ModelLoader IS movable
		ModelLoader(ModelLoader &&) noexcept = default;
		ModelLoader & operator=(ModelLoader &&) noexcept = default;
	private:
		std::string project_path_;
	};
}
