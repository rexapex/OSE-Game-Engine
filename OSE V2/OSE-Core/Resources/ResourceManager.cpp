#include "stdafx.h"
#include "ResourceManager.h"
#include "OSE-Core/EngineReferences.h"
#include "OSE-Core/Rendering/RenderingFactory.h"
#include "OSE-Core/Resources/Texture/TextureLoaderFactory.h"
#include "Texture/Texture.h"
#include "Texture/TextureLoader.h"
#include "Texture/TextureMetaData.h"
#include "Tilemap/TilemapLoaderFactory.h"
#include "Tilemap/Tilemap.h"
#include "Tilemap/TilemapLoader.h"
#include "FileHandlingUtil.h"

namespace ose
{
	ResourceManager::ResourceManager(const std::string & project_path) : project_path_(project_path),
		texture_loader_(TextureLoaderFactories[0]->NewTextureLoader(project_path)),
		tilemap_loader_(TilemapLoaderFactories[0]->NewTilemapLoader(project_path)) {}
	ResourceManager::~ResourceManager() noexcept {}

	ResourceManager::ResourceManager(ResourceManager && other) noexcept : project_path_(std::move(other.project_path_)),
		textures_without_Gpu_memory_(std::move(other.textures_without_Gpu_memory_)), textures_with_Gpu_memory_(std::move(other.textures_with_Gpu_memory_)),
		texture_loader_(std::move(other.texture_loader_)), tilemap_loader_(std::move(other.tilemap_loader_)) {}

	ResourceManager & ResourceManager::operator=(ResourceManager && other) noexcept
	{
		project_path_ = std::move(other.project_path_);
		textures_without_Gpu_memory_ = std::move(other.textures_without_Gpu_memory_);
		textures_with_Gpu_memory_ = std::move(other.textures_with_Gpu_memory_);
		texture_loader_ = std::move(other.texture_loader_);
		tilemap_loader_ = std::move(other.tilemap_loader_);
		return *this;
	}

	//import a file into the project resources directory
	//sub_dir is a sub directory within the resources directory
	void ResourceManager::ImportFile(const std::string & file_path, const std::string & sub_dir)
	{
		//TODO - don't accept .meta files
		//TODO - auto generate a .meta file for the new resources if successfully imported (but I don't know it's type here!!!)
		FileHandlingUtil::CopyFile(file_path, project_path_ + "/Resources/" + sub_dir + "/" + FileHandlingUtil::GetFilenameFromPath(file_path));
	}

	//imports multiple files into project resources directory
	//sub_dir is a sub directory within the resources directory
	void ResourceManager::ImportFiles(const std::vector<std::string> & file_paths, const std::string & sub_dir)
	{
		for(auto & path : file_paths)
		{
			ImportFile(path, sub_dir);
		}
	}

	// get the texture from either map
	// given the name of the texture, return the texture object
	unowned_ptr<Texture const> ResourceManager::GetTexture(const std::string name)
	{
		// search the textures_with_GPU_memory_ list
		auto const & tex_iter { textures_with_Gpu_memory_.find(name) };
		if(tex_iter != textures_with_Gpu_memory_.end()) {
			return tex_iter->second.get();
		}

		// then, if texture couldn't be found, search the textures_without_GPU_memory_ list
		auto const & tex_iter2 { textures_without_Gpu_memory_.find(name) };
		if(tex_iter2 != textures_without_Gpu_memory_.end()) {
			return tex_iter2->second.get();
		}

		return nullptr;
	}

	// adds the texture at path to the list of active textures, the texture must be in the project's resources directory
	// path is relative to ProjectPath/Resources
	// if no name is given, the filepath will be used
	// TODO - either remove name altogether or come up with something clever
	void ResourceManager::AddTexture(const std::string & path, const std::string & name)
	{
		std::string abs_path { project_path_ + "/Resources/" + path };

		if(FileHandlingUtil::DoesFileExist(abs_path))
		{
			// if no name is given, use the filename
			std::string name_to_use { name };
			if(name_to_use == "")
			{
				name_to_use = path;//FileHandlingUtil::filenameFromPath(abs_path);
			}

			// only add the new texture if the name is not taken (in either map)
			auto & iter = textures_without_Gpu_memory_.find(name_to_use);
			auto & iter2 = textures_with_Gpu_memory_.find(name_to_use);
			if(iter == textures_without_Gpu_memory_.end() && iter2 == textures_with_Gpu_memory_.end())
			{
				textures_without_Gpu_memory_.emplace(name_to_use, RenderingFactories[0]->NewTexture(name_to_use, abs_path));
				DEBUG_LOG("Added texture " << name_to_use << " to ResourceManager");
				
				// get a references to the newly created texture
				auto & tex = textures_without_Gpu_memory_.at(name_to_use);

				// load or generate the texture's meta data
				std::string meta_abs_path { abs_path + ".meta" };
				bool success = false;
				TextureMetaData meta_data;	//object will have default values
				if(FileHandlingUtil::DoesFileExist(meta_abs_path))
				{
					//load meta data file
					try
					{
						LoadTextureMetaFile(meta_abs_path, meta_data);
						success = true;
					}
					catch(const std::exception &) {}	// success will be false, therefore, meta file will be created
				}
				else if(!success)
				{
					// create meta data file
					// NOTE - compiler auto concatenates adjacent string literals
					FileHandlingUtil::WriteTextFile(meta_abs_path,	"mag_filter_mode 0\n"
																	"min_filter_mode 0\n"
																	"mip_mapping_enabled 1\n"
																	"min_LOD 0\n"
																	"max_LOD 0\n"
																	"LOD_bias 0");
				}

				// set the texture's meta data
				tex->SetMetaData(meta_data);

				// TODO - do the loading with multi-threading
				int32_t w, h;
				IMGDATA d;
				texture_loader_->LoadTexture(abs_path, &d, &w, &h);
				tex->SetImgData(d, w, h);
			}
			else
			{
				LOG("error: texture name " + name_to_use + " is already taken");
			}
		}
	}

	// create the GPU memory for an already loaded (added) texture
	// returns an iterator to the next texture in the textures_without_GPU_memory map
	// IMPORANT - can only be called from the thread which contains the render context
	std::map<std::string, std::unique_ptr<Texture>>::const_iterator ResourceManager::CreateTexture(const std::string & tex_name)
	{
		// get the texture if it exists
		// only texture with no representation in GPU memory can be created
		auto const & tex_iter { textures_without_Gpu_memory_.find(tex_name) };

		// if the texture exists, then create its GPU representation
		if(tex_iter != textures_without_Gpu_memory_.end())
		{
			try {
				// try to create the texture on the GPU
				tex_iter->second->CreateTexture();
				// iff the creation succeeds, move the texture from one map to the other
				textures_with_Gpu_memory_.insert({ tex_name, std::move(tex_iter->second) });
				// remove the texture from the original map
				return textures_without_Gpu_memory_.erase(tex_iter);
			} catch(const std::exception & e) {
				ERROR_LOG(e.what());
			}
		}

		return tex_iter;
	}

	// remove the texture from the textures list and free the texture's resources
	// IMPORANT - can only be called from the thread which contains the render context
	void ResourceManager::RemoveTexture(const std::string & tex_name)
	{
		// get the texture if it exists
		// only texture without memory allocated on the GPU can be removed
		auto const & tex_iter { textures_without_Gpu_memory_.find(tex_name) };

		// if the texture exists, then free its resources and remove it from map
		if(tex_iter != textures_without_Gpu_memory_.end())
		{
			// free the image data resource
			texture_loader_->FreeTexture(tex_iter->second->GetImgData());
			// remove the texture from the map
			textures_without_Gpu_memory_.erase(tex_iter);
		}
	}

	// free the GPU memory of the texture
	// IMPORANT - can only be called from the thread which contains the render context
	void ResourceManager::DestroyTexture(const std::string & tex_name)
	{
		// get the texture if it exists
		// only texture with memory allocated on the GPU can be destroyed
		auto const & tex_iter { textures_with_Gpu_memory_.find(tex_name) };

		// if the texture exists, then free its resources and remove it from map
		if(tex_iter != textures_with_Gpu_memory_.end())
		{
			// free the image data resource
			tex_iter->second->DestroyTexture();
			// iff the creation succeeds, move the texture from one map to the other
			textures_without_Gpu_memory_.insert({ tex_name, std::move(tex_iter->second) });
			// remove the texture from the map
			textures_with_Gpu_memory_.erase(tex_iter);
		}
	}

	// create all textures which are currently lacking a GPU representation
	// IMPORTANT - can only be called from the thread which contains the render context
	void ResourceManager::CreateTextures()
	{
		std::map<std::string, std::unique_ptr<Texture>>::const_iterator it;

		// create a GPU texture for each texture without a GPU texture representation
		for(it = textures_without_Gpu_memory_.begin(); it != textures_without_Gpu_memory_.end(); )
		{
			// delegate to this method because why not
			it = CreateTexture(it->first);
		}
	}

	//loads a meta file for some texture, meta files map properties to values
	void ResourceManager::LoadTextureMetaFile(const std::string & abs_path, TextureMetaData & meta_data)
	{
		//load the meta data string
		std::string contents;
		try
		{
			FileHandlingUtil::LoadTextFile(abs_path, contents);
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
				meta_data.min_lod_ = value;
			} else if(property == "max_LOD") {
				meta_data.max_lod_ = value;
			} else if(property == "LOD_bias") {
				meta_data.lod_bias_ = value;
			}
		}
	}

	// Get the tilemap from the resources manager
	// Given the name of the tilemap, return the tilemap object
	ose::unowned_ptr<Tilemap const> ResourceManager::GetTilemap(const std::string & name)
	{
		// search the tilemaps_ list
		auto const & iter { tilemaps_.find(name) };
		if(iter != tilemaps_.end()) {
			return iter->second.get();
		}

		return nullptr;
	}

	// Adds the tilemap at path to the list of active tilemaps, the tilemap must be in the project's resources directory
	// Path is relative to ProjectPath/Resources
	// If no name is given, the relative path will be used
	// IMPORTANT - Can be called from any thread (TODO)
	void ResourceManager::AddTilemap(const std::string & path, const std::string & name)
	{
		std::string abs_path { project_path_ + "/Resources/" + path };

		if(FileHandlingUtil::DoesFileExist(abs_path))
		{
			// if no name is given, use the filename
			std::string name_to_use { name };
			if(name_to_use == "")
			{
				name_to_use = path;//FileHandlingUtil::filenameFromPath(abs_path);
			}

			// only add the new tilemaps if the name is not taken
			auto & iter = tilemaps_.find(name_to_use);
			if(iter == tilemaps_.end())
			{
				tilemaps_.emplace(name_to_use, std::make_unique<Tilemap>(name_to_use, abs_path));
				DEBUG_LOG("Added tilemap " << name_to_use << " to ResourceManager");

				// get a references to the newly created tilemap
				auto & tilemap = tilemaps_.at(name_to_use);

				// TODO - Do the loading with multi-threading
				tilemap_loader_->LoadTilemap(abs_path, *tilemap);
			}
			else
			{
				LOG("error: tilemap name " + name_to_use + " is already taken");
			}
		}
	}

	// Remove the tilemap from the tilemaps list and free the tilemap's resources
	// IMPORANT - can be called from any thread (TODO)
	void ResourceManager::RemoveTilemap(const std::string & name)
	{
		// Get the tilemap if it exists
		auto const & iter { tilemaps_.find(name) };

		// If the tilemaps exists, remove it from the map
		if(iter != tilemaps_.end())
		{
			// Remove the texture from the map
			tilemaps_.erase(iter);
		}
	}
}
