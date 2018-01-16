#include "ResourceManager.h"

namespace origami_sheep_engine
{
	ResourceManager::ResourceManager(const std::string & project_path) : project_path_(project_path), texture_loader_(std::make_unique<TextureLoaderImpl>(project_path)) {}
	ResourceManager::~ResourceManager() noexcept {}

	ResourceManager::ResourceManager(ResourceManager && other) noexcept : project_path_(std::move(other.project_path_)),
									name_to_tex_map_(std::move(other.name_to_tex_map_)), texture_loader_(std::move(other.texture_loader_)) {}

	ResourceManager & ResourceManager::operator=(ResourceManager && other) noexcept
	{
		project_path_ = std::move(other.project_path_);
		name_to_tex_map_ = std::move(other.name_to_tex_map_);
		texture_loader_ = std::move(other.texture_loader_);
		return *this;
	}

	//import a file into the project resources directory
	//sub_dir is a sub directory within the resources directory
	void ResourceManager::importFile(const std::string & file_path, const std::string & sub_dir)
	{
		//TODO - don't accept .meta files
		//TODO - auto generate a .meta file for the new resources if successfully imported
		FileHandlingUtil::copyFile(file_path, project_path_ + "/Resources/" + sub_dir + "/" + FileHandlingUtil::filenameFromPath(file_path));
	}

	//imports multiple files into project resources directory
	//sub_dir is a sub directory within the resources directory
	void ResourceManager::importFiles(const std::vector<std::string> & file_paths, const std::string & sub_dir)
	{
		for(auto & path : file_paths)
		{
			importFile(path, sub_dir);
		}
	}

	//adds the texture at path to the list of active textures, the texture must be in the project's resources directory
	//path is relative to ProjectPath/Resources
	//if no name is given, the filename will be used
	void ResourceManager::addTexture(const std::string & path, const std::string & name)
	{
		std::string abs_path { project_path_ + "/Resources/" + path };

		if(FileHandlingUtil::doesFileExist(abs_path))
		{
			//if no name is given, use the filename
			std::string name_to_use { name };
			if(name_to_use == "")
			{
				name_to_use = FileHandlingUtil::filenameFromPath(abs_path);
			}

			//only add the new texture if the name is not taken
			auto & iter = name_to_tex_map_.find(name_to_use);
			if(iter == name_to_tex_map_.end())
			{
				name_to_tex_map_.emplace(name_to_use, Texture ( name_to_use, abs_path ));
				
				Texture & tex = name_to_tex_map_.at(name_to_use);	//get a references to the newly created texture

				//TODO - do the loading multi-threading
				int32_t w, h;
				IMGDATA d;
				texture_loader_->loadTexture(abs_path, &d, &w, &h);
				tex.set_img_data(d, w, h);
			}
			else
			{
				LOG("error: texture name " + name_to_use + " is already taken");
			}
		}
	}

	//remove the texture from the textures list and free the texture's resources
	void ResourceManager::removeTexture(const std::string & tex_name)	//remove by texture name
	{
		//get the texture if it exists
		auto const & tex_iter { name_to_tex_map_.find(tex_name) };

		//if the texture exists, then free its resources and remove it from map
		if(tex_iter != name_to_tex_map_.end())
		{
			//free the image data resource
			texture_loader_->freeTexture(tex_iter->second);
			//remove the texture from the map
			name_to_tex_map_.erase(tex_iter);
		}
	}

	void ResourceManager::removeTexture(const Texture & tex)			//remove by element
	{
		//defer processing to remove texture by name function
		//assumes texture names are unique
		removeTexture(tex.get_name());
	}
}
