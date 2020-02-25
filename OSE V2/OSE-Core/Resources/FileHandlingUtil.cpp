#include "stdafx.h"
#include "FileHandlingUtil.h"

#include <fstream>
#include <filesystem>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
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
	void FileHandlingUtil::LoadTextFile(const std::string & path, std::string & text)
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

	// Writes text file at 'path' with the contents 'text'
	// The file will be created if it does not already exist
	void FileHandlingUtil::WriteTextFile(const std::string & path, const std::string & text)
	{
		// https://stackoverflow.com/questions/478075/creating-files-in-c
		std::ofstream out(path);
		out << text << std::endl;
		out.close();
	}

	void FileHandlingUtil::GetHomeDirectory(std::string & home_dir_path)
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
		// Get windows Documents folder
		CHAR docs_path[MAX_PATH];
		HRESULT result = SHGetFolderPathA(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, docs_path);

		if(result == S_OK)
		{
			home_dir_path = docs_path;
		}
		else
		{
			ERROR_LOG("Error: Could not find HOME directory");
		}

		//get windows Documents folder
		//https://stackoverflow.com/questions/15916695/can-anyone-give-me-example-code-of-dupenv-s
		/*char * buffer;
		size_t size;
		if(_dupenv_s(&buffer, &size, "USERPROFILE") == 0 && buffer != nullptr)
		{
			std::string path(buffer);
			path += "\\Documents";
			home_dir_path = path;
			free(buffer);
		}*/
	#endif
	}

	// Get the directory of the executable
	std::string FileHandlingUtil::GetExecutableDirectory()
	{
#		ifdef _WIN32
			CHAR filename[MAX_PATH];
			DWORD size = GetModuleFileNameA(NULL, filename, MAX_PATH);
			if(size > 0)
			{
				return GetParentPathFromPath(filename);
			}
#		else
#			error FileHandlingUtil::GetExecutableDirectory only supported on Windows
#		endif
		return "";
	}

	//Copy the file at the from path to the to path
	void FileHandlingUtil::CopyFile_(const std::string & from, const std::string & to)
	{
		try
		{
			//first, create the necessary directories
			std::filesystem::create_directories(GetParentPathFromPath(to));

			//then, copy the file
			bool success = std::filesystem::copy_file(from, to);

			if(success)
			{
				LOG("success: " + from + " copied successfully to " + to);
			}
			else
			{
				LOG("error: " + from + " could not be copied to " + to);
			}
		}
		catch(std::filesystem::filesystem_error &)
		{
			LOG("error: invalid arguments");
		}
	}

	//Creates directories given in path if they do not already exist
	void FileHandlingUtil::CreateDirs(const std::string & path)
	{
		auto & p = std::filesystem::path(path);
		if(p.has_filename())
		{
			std::filesystem::create_directories(GetParentPathFromPath(path));
		}
		else
		{
			std::filesystem::create_directories(path);
		}
	}

	//Returns true iff the path exists and is a file
	bool FileHandlingUtil::DoesFileExist(const std::string & path)
	{
		return std::filesystem::exists(path) && std::filesystem::is_regular_file(path);
	}

	//Get the filename of a path
	std::string FileHandlingUtil::GetFilenameFromPath(const std::string & path)
	{
		return std::filesystem::path(path).filename().string();
	}

	//Get the parent path of a path
	std::string FileHandlingUtil::GetParentPathFromPath(const std::string & path)
	{
		return std::filesystem::path(path).parent_path().string();
	}

	// Get a field from the engine's settings ini file
	// The field must be of the type string
	std::string FileHandlingUtil::GetEngineSettingsString(const std::string & section, const std::string & name, const std::string & default_value)
	{
		// TODO - Make my own INI parse and manager, probably will be easier than this

	#ifdef WIN32
		/*std::wstring ack = L"foo";
		LPCTSTR section_, name_, default_value_;
		swprintf(ack.c_str(), section.size(), L"%hs", section);
		GetPrivateProfileString(section.c_str(), "server", "127.0.0.1", dbserver, sizeof(dbserver) / sizeof(dbserver[0]), ".\\dbsettings.ini");*/
	#endif
		return "";
	}
}
