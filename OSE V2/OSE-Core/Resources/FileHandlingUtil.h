#pragma once
#include <fstream>
#include <filesystem>

#ifdef _WIN32
#include <windows.h>
#include <ShlObj_core.h>
#endif

#if defined(__APPLE__) || defined(__linux__)
#include <pwd.h>
#include <unistd.h>
#include <sys/types.h>
#endif

namespace ose
{
	class FileHandlingUtil
	{
	public:
		FileHandlingUtil() = delete;
		~FileHandlingUtil() = default;

		// Loads the text file at 'path' and stores text in 'text'
		// @param {std::string &} path The relative? path of the file to load
		// @param {std::string &} text The string to be filled with the file's text
		static void LoadTextFile(const std::string & path, std::string & text);

		// Writes text file at 'path' with the contents 'text'
		// The file will be created if it does not already exist
		static void WriteTextFile(const std::string & path, const std::string & text);

		// Get the users home directory
		// Supports compile on Windows, Linux, (TODO MacOS) using ifdef
		// Returns Documents folder on Windows
		// WARNING: NOT THREAD SAFE
		static void GetHomeDirectory(std::string & home_dir_path);

		// Copy the file at the from path to the to path
		// Will create the necessary destination directories
		static void CopyFile(const std::string & from, const std::string & to);

		// Creates directories given in path if they do not already exist
		static void CreateDirs(const std::string & path);

		// Returns true iff the path exists and is a file
		static bool DoesFileExist(const std::string & path);

		// Returns the current working directory
		static std::string GetCWD() { return std::filesystem::current_path().string(); }

		// Get the filename of a path
		static std::string GetFilenameFromPath(const std::string & path);

		// Get the parent path of a path
		static std::string GetParentPathFromPath(const std::string & path);

		// Get a field from the engine's settings ini file
		// The field must be of the type string
		static std::string GetEngineSettingsString(const std::string & section, const std::string & name, const std::string & default_value);
	};
}
