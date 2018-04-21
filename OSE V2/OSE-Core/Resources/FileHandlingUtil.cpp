#include "../../stdafx.h"
#include "FileHandlingUtil.h"

void FileHandlingUtil::loadTextFile(const std::string & path, std::string & text)
{
	std::ifstream in(path.c_str(), std::ios::in | std::ios::binary);

	if(in)
	{
		in.seekg(0, std::ios::end);
		text.resize(static_cast<size_t>(in.tellg()));
		in.seekg(0, std::ios::beg);
		in.read(&text[0], text.size());
		in.close();
	}
	else
	{
		throw std::invalid_argument("ERROR: could not open file, " + path);
	}
}

//Writes text file at 'path' with the contents 'text'
//The file will be created if it does not already exist
void FileHandlingUtil::writeTextFile(const std::string & path, const std::string & text)
{
	//https://stackoverflow.com/questions/478075/creating-files-in-c
	std::ofstream out(path);
	out << text << std::endl;
	out.close();
}

void FileHandlingUtil::getHomeDirectory(std::string & home_dir_path)
{
#if defined(__APPLE__) || defined(__linux__)
	//get linux/mac HOME environment variable
	const char * home_dir = getenv("HOME");
	if(home_dir == NULL)
	{
		std::cerr << "LINUX FileHandlingUtil::getHomeDirectory -> home_dir is NULL" << std::endl;
		//not thread safe
		home_dir = getpwuid(getuid()) != NULL ? getpwuid(getuid())->pw_dir : "/";
	}

	home_dir_path = std::string(home_dir);
#elif defined(_WIN32)
	//get windows Documents folder
	//https://stackoverflow.com/questions/15916695/can-anyone-give-me-example-code-of-dupenv-s
	char * buffer;
	size_t size;
	if(_dupenv_s(&buffer, &size, "USERPROFILE") == 0 && buffer != nullptr)
	{
		std::string path(buffer);
		path += "\\Documents";
		home_dir_path = path;
		free(buffer);
	}
#endif
}

//Copy the file at the from path to the to path
void FileHandlingUtil::copyFile(const std::string & from, const std::string & to)
{
	try
	{
		//first, create the necessary directories
		std::experimental::filesystem::create_directories(parentPathFromPath(to));

		//then, copy the file
		bool success = std::experimental::filesystem::copy_file(from, to);

		if(success)
		{
			LOG("success: " + from + " copied successfully to " + to);
		}
		else
		{
			LOG("error: " + from + " could not be copied to " + to);
		}
	}
	catch(std::experimental::filesystem::filesystem_error &)
	{
		LOG("error: invalid arguments");
	}
}

//Creates directories given in path if they do not already exist
void FileHandlingUtil::createDirs(const std::string & path)
{
	auto & p = std::experimental::filesystem::path(path);
	if(p.has_filename())
	{
		std::experimental::filesystem::create_directories(parentPathFromPath(path));
	}
	else
	{
		std::experimental::filesystem::create_directories(path);
	}
}

//Returns true iff the path exists and is a file
bool FileHandlingUtil::doesFileExist(const std::string & path)
{
	return std::experimental::filesystem::exists(path) && std::experimental::filesystem::is_regular_file(path);
}

//Get the filename of a path
std::string FileHandlingUtil::filenameFromPath(const std::string & path)
{
	return std::experimental::filesystem::path(path).filename().string();
}

//Get the parent path of a path
std::string FileHandlingUtil::parentPathFromPath(const std::string & path)
{
	return std::experimental::filesystem::path(path).parent_path().string();
}