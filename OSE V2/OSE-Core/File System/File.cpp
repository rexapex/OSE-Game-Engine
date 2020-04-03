#include "stdafx.h"
#include "File.h"
#include "FileSystemUtil.h"

namespace ose::fs
{
	File::File(std::string const & path) : path_(path)
	{
	
	}

	File::~File()
	{
		Close();
	}

	// Creates the file and any required parent folders
	// Returns true if the file is created or if the file already exists
	bool File::Create()
	{
		if(Exists())
			return true;
		return CreateFile_(path_);
	}

	// Returns true iff the file exists
	bool File::Exists()
	{
		return DoesFileExist(path_);
	}
	
	// Opens a stream to the file object for reading/writing/appending
	void File::Open(int flags)
	{
		if(Create())
		{
			int f = (((flags & FF_READ) == FF_READ) ? std::fstream::in : 0) |
					(((flags & FF_WRITE) == FF_WRITE) ? std::fstream::out : 0) |
					(((flags & FF_APPEND) == FF_APPEND) ? std::fstream::app : 0) |
					(((flags & FF_BINARY) == FF_BINARY) ? std::fstream::binary : 0);
			stream_.open(path_, f);
		}
	}

	// Closes the open stream to the file (if there is one)
	void File::Close()
	{
		if(stream_.is_open())
			stream_.close();
	}
}
