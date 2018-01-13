#pragma once

#include "stdafx.h"
#include "Texture.h"

namespace origami_sheep_engine
{
	class ResourceManager
	{
	public:
		ResourceManager(const std::string & project_path);
		~ResourceManager() noexcept;
		//copying is not allowed
		ResourceManager(ResourceManager &) = delete;
		ResourceManager & operator=(ResourceManager &) = delete;
		//moving is allowed
		ResourceManager(ResourceManager &&) noexcept = default;
		ResourceManager & operator=(ResourceManager &&) noexcept = default;

		//import a file into the project resources directory
		//sub_dir is a sub directory within the resources directory
		void importFile(const std::string & file_path, const std::string & sub_dir = "");

		//imports multiple files into project resources directory
		//sub_dir is a sub directory within the resources directory
		void importFiles(const std::vector<std::string> & file_paths, const std::string & sub_dir = "");

		//adds the texture at path to the list of active textures, the texture must be in the project's resources directory
		//path is relative to ProjectPath/Resources
		//if no name is given, the filename will be used
		void addTexture(const std::string & path, const std::string & name = "");

		//remove the texture from the textures list and free the texture's resources
		void removeTexture(const uint32_t index);	//remove by index
		void removeTexture(const Texture & tex);	//remove by element

	private:
		std::string project_path_;
	};
}
