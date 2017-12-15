#pragma once
#include <fstream>

#if defined(__APPLE__) || defined(__linux__)
#include <pwd.h>
#include <unistd.h>
#include <sys/types.h>
#endif

class FileHandlingUtil
{
public:
	FileHandlingUtil() = delete;
	~FileHandlingUtil() = default;

	//Loads the text file at 'path' and stores text in 'text'
	//@param {std::string &} path The relative? path of the file to load
	//@param {std::string &} text The string to be filled with the file's text
	static void loadTextFile(const std::string & path, std::string & text);

	//Get the users home directory
	//Supports compile on Windows, Linux, (TODO MacOS) using ifdef
	//Returns Documents folder on Windows
	//WARNING: NOT THREAD SAFE
	static void getHomeDirectory(std::string & home_dir_path);
};

