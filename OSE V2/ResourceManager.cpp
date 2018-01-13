#include "ResourceManager.h"

namespace origami_sheep_engine
{
	ResourceManager::ResourceManager(const std::string & project_path) : project_path_(project_path) {}
	ResourceManager::~ResourceManager() noexcept {}

	//import a file into the project resources directory
	//sub_dir is a sub directory within the resources directory
	void ResourceManager::importFile(const std::string & file_path, const std::string & sub_dir)
	{
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
		//TODO
	}

	//remove the texture from the textures list and free the texture's resources
	void ResourceManager::removeTexture(const uint32_t index)		//remove by index
	{
		//TODO
	}

	void ResourceManager::removeTexture(const Texture & tex)		//remove by element
	{
		//TODO
	}
}
