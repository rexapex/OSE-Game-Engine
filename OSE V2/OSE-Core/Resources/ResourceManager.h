#pragma once

#include "../../stdafx.h"
#include "FileHandlingUtil.h"
#include "Texture.h"
#include "TextureLoader.h"
#include "TextureMetaData.h"
#include <sstream>

namespace ose::resources
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
		ResourceManager(ResourceManager &&) noexcept;
		ResourceManager & operator=(ResourceManager &&) noexcept;

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
		void removeTexture(const std::string & tex_name);	//remove by texture name
		void removeTexture(const Texture & tex);			//remove by element

		//loads a meta file for some texture, meta files map properties to values
		void loadTextureMetaFile(const std::string & abs_path, TextureMetaData & meta_data);

	private:
		//the root path of the currently loaded project
		std::string project_path_;

		//maps texture name to texture object
		std::map<std::string, Texture> name_to_tex_map_;

		//the TextureLoader object used for loading textures from image files
		std::unique_ptr<TextureLoader> texture_loader_;
	};
}
