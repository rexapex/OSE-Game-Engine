#include "stdafx.h"
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
