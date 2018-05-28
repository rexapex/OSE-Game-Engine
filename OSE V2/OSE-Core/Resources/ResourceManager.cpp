#include "stdafx.h"
#include "ResourceManager.h"

namespace ose::resources
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
		//TODO - auto generate a .meta file for the new resources if successfully imported (but I don't know it's type here!!!)
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

	// adds the texture at path to the list of active textures, the texture must be in the project's resources directory
	// path is relative to ProjectPath/Resources
	// if no name is given, the filepath will be used
	// TODO - either remove name altogether or come up with something clever
	void ResourceManager::addTexture(const std::string & path, const std::string & name)
	{
		std::string abs_path { project_path_ + "/Resources/" + path };

		if(FileHandlingUtil::doesFileExist(abs_path))
		{
			//if no name is given, use the filename
			std::string name_to_use { name };
			if(name_to_use == "")
			{
				name_to_use = path;//FileHandlingUtil::filenameFromPath(abs_path);
			}

			//only add the new texture if the name is not taken
			auto & iter = name_to_tex_map_.find(name_to_use);
			if(iter == name_to_tex_map_.end())
			{
				name_to_tex_map_.emplace(name_to_use, Texture ( name_to_use, abs_path ));
				
				Texture & tex = name_to_tex_map_.at(name_to_use);	//get a references to the newly created texture

				//load or generate the texture's meta data
				std::string meta_abs_path { abs_path + ".meta" };
				bool success = false;
				TextureMetaData meta_data;	//object will have default values
				if(FileHandlingUtil::doesFileExist(meta_abs_path))
				{
					//load meta data file
					try
					{
						loadTextureMetaFile(meta_abs_path, meta_data);
						success = true;
					}
					catch(const std::exception &) {}	//success will be false, therefore, meta file will be created
				}
				else if(!success)
				{
					//create meta data file
					//NOTE - compiler auto concatenates adjacent string literals
					FileHandlingUtil::writeTextFile(meta_abs_path,	"mag_filter_mode 0\n"
																	"min_filter_mode 0\n"
																	"mip_mapping_enabled 1\n"
																	"min_LOD 0\n"
																	"max_LOD 0\n"
																	"LOD_bias 0");
				}

				//set the texture's meta data
				tex.set_meta_data(meta_data);

				//TODO - do the loading with multi-threading
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

	//loads a meta file for some texture, meta files map properties to values
	void ResourceManager::loadTextureMetaFile(const std::string & abs_path, TextureMetaData & meta_data)
	{
		//load the meta data string
		std::string contents;
		try
		{
			FileHandlingUtil::loadTextFile(abs_path, contents);
		}
		catch(const std::exception & e)
		{
			//error occurred, therefore, return an empty project info stub
			LOG("FileHandlingUtil::load_text_file -> " << e.what());
			throw e;
		}

		std::stringstream iss { contents };
		//yucky solution but if it works it works
		while(iss)
		{
			std::string property;
			iss >> property;
			uint32_t value;
			iss >> value;

			if(property == "mag_filter_mode") {
				meta_data.mag_filter_mode_ = static_cast<ETextureFilterMode>(value);
			} else if(property == "min_filter_mode") {
				meta_data.min_filter_mode_ = static_cast<ETextureFilterMode>(value);
			} else if(property == "mip_mapping_enabled") {
				meta_data.mip_mapping_enabled_ = static_cast<bool>(value);
			} else if(property == "min_LOD") {
				meta_data.min_LOD_ = value;
			} else if(property == "max_LOD") {
				meta_data.max_LOD_ = value;
			} else if(property == "LOD_bias") {
				meta_data.LOD_bias_ = value;
			}
		}
	}
}
