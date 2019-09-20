#pragma once

#include "OSE-Core/Types.h"
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
		// copying is not allowed
		ResourceManager(ResourceManager &) = delete;
		ResourceManager & operator=(ResourceManager &) = delete;
		// moving is allowed
		ResourceManager(ResourceManager &&) noexcept;
		ResourceManager & operator=(ResourceManager &&) noexcept;

		// import a file into the project resources directory
		// sub_dir is a sub directory within the resources directory
		void ImportFile(const std::string & file_path, const std::string & sub_dir = "");

		// imports multiple files into project resources directory
		// sub_dir is a sub directory within the resources directory
		void ImportFiles(const std::vector<std::string> & file_paths, const std::string & sub_dir = "");

		// get the texture from either map
		// given the name of the texture, return the texture object
		ose::unowned_ptr<Texture const> GetTexture(const std::string name);

		// adds the texture at path to the list of active textures, the texture must be in the project's resources directory
		// path is relative to ProjectPath/Resources
		// if no name is given, the relative path will be used
		// IMPORTANT - can be called from any thread
		void AddTexture(const std::string & path, const std::string & name = "");

		// create the GPU memory for an already loaded (added) texture
		// returns an iterator to the next texture in the textures_without_GPU_memory map
		// IMPORANT - can only be called from the thread which contains the render context
		std::map<std::string, std::unique_ptr<Texture>>::const_iterator CreateTexture(const std::string & tex_name);

		// remove the texture from the textures list and free the texture's resources
		// IMPORANT - can be called from any thread
		void RemoveTexture(const std::string & tex_name);

		// free the GPU memory of the texture
		// IMPORANT - can only be called from the thread which contains the render context
		void DestroyTexture(const std::string & tex_name);

		// create all textures which are currently lacking a GPU representation
		// IMPORTANT - can only be called from the thread which contains the render context
		void CreateTextures();

		// loads a meta file for some texture, meta files map properties to values
		void LoadTextureMetaFile(const std::string & abs_path, TextureMetaData & meta_data);

	private:
		// the root path of the currently loaded project
		std::string project_path_;

		// maps texture name to texture object
		// any texture with a representation in GPU memory
		// these textures can be destroyed but not removed
		std::map<std::string, std::unique_ptr<Texture>> textures_with_Gpu_memory_;

		// maps texture name to texture object
		// any texture with no representation in GPU memory
		// only these textures can be created and removed
		std::map<std::string, std::unique_ptr<Texture>> textures_without_Gpu_memory_;

		// the TextureLoader object used for loading textures from image files
		std::unique_ptr<TextureLoader> texture_loader_;
	};
}
