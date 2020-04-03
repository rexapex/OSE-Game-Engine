#pragma once

namespace ose
{
	namespace fs
	{
		// Loads the text file at 'path' and stores text in 'text'
		// @param {std::string &} path The relative? path of the file to load
		// @param {std::string &} text The string to be filled with the file's text
		void LoadTextFile(const std::string & path, std::string & text);

		// Writes text file at 'path' with the contents 'text'
		// The file will be created if it does not already exist
		void WriteTextFile(const std::string & path, const std::string & text);

		// Get the users home directory
		// Supports compile on Windows, Linux, (TODO MacOS) using ifdef
		// Returns Documents folder on Windows
		// WARNING: NOT THREAD SAFE
		void GetHomeDirectory(std::string & home_dir_path);

		// Get the directory of the executable
		std::string GetExecutableDirectory();

		// Copy the file at the from path to the to path
		// Will create the necessary destination directories
		void CopyFile_(const std::string & from, const std::string & to);

		// Creates directories given in path if they do not already exist
		void CreateDirs(const std::string & path);

		// Returns true iff the path exists and is a file
		bool DoesFileExist(const std::string & path);

		// Get the filename of a path
		std::string GetFilenameFromPath(const std::string & path);

		// Get the parent path of a path
		std::string GetParentPath(const std::string & path);
	}
}
