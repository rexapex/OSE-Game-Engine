#include "stdafx.h"
#include "FileHandlingUtil.h"

void FileHandlingUtil::loadTextFile(const std::string & path, std::string & text)
{
	std::ifstream in(path.c_str(), std::ios::in | std::ios::binary);

	if(in)
	{
		in.seekg(0, std::ios::end);
		text.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&text[0], text.size());
		in.close();
	}
	else
	{
		throw "ERROR: could not open file, " + path;
	}
}
