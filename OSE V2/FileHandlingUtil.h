#pragma once
#include <fstream>

class FileHandlingUtil
{
public:
	FileHandlingUtil() = delete;
	~FileHandlingUtil() = default;

	//Loads the text file at 'path' and stores text in 'text'
	//@param {std::string &} path The relative? path of the file to load
	//@param {std::string &} text The string to be filled with the file's text
	static void loadTextFile(const std::string & path, std::string & text);
};

