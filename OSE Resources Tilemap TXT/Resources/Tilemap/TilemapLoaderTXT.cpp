#include "pch.h"
#include "TilemapLoaderTXT.h"
#include "OSE-Core/File System/FileSystemUtil.h"

namespace ose::resources
{
	TilemapLoaderTXT::TilemapLoaderTXT(std::string const & project_path) : TilemapLoader(project_path) {}

	TilemapLoaderTXT::~TilemapLoaderTXT() {}

	// Loads the tilemap and sets the internal data of the tilemap object
	// Path is absolute and is guaranteed to exist
	void TilemapLoaderTXT::LoadTilemap(std::string const & path, Tilemap & tilemap)
	{
		if(fs::DoesFileExist(path))
		{
			// Load the text file
			std::string text;
			try
			{
				fs::LoadTextFile(path, text);
			}
			catch(...)
			{
				LOG_ERROR("Failed to load tilemap", path);
				return;
			}
			
			// Split the text into lines and calculate the height
			std::stringstream text_ss(text);
			std::string line;
			std::vector<std::string> lines;
			while(std::getline(text_ss, line))
			{
				if(line.size() > 0)
					lines.push_back(line);
			}
			int32_t height = static_cast<int32_t>(lines.size());

			// Calculate the width
			int32_t width { 0 };
			if(lines.size() > 0)
			{
				std::stringstream line_ss(line);
				std::string cell;
				while(std::getline(line_ss, cell, ' '))
				{
					if(cell.size() > 0)
						width++;
				}
			}

			// Create the tilemap data
			tilemap.CreateTileGrid(width, height);

			// Split each line into cells and fill the tilemap
			for(size_t y = 0; y < lines.size(); y++)
			{
				std::string & line = lines[y];
				std::stringstream line_ss(line);
				std::string cell;
				size_t x { 0 };
				while(std::getline(line_ss, cell, ' '))
				{
					if(cell.size() > 0)
					{
						// Ensure the cell index is not out of bounds
						if(x >= width)
						{
							LOG_ERROR("Invalid tilemap file, varying row length");
							tilemap.DeleteTileGrid();
							return;
						}
						// Parse the cell value
						try
						{
							int32_t value = static_cast<int32_t>(std::stoi(cell));
							tilemap(x, y) = value;
						}
						catch(...)
						{
							LOG_ERROR("Invalid tilemap file, cell is not a number");
							tilemap.DeleteTileGrid();
							return;
						}
						// Advance the cell index
						x++;
					}
				}
			}
		}
		else
		{
			LOG_ERROR("Tilemap", path, "does not exist");
		}
	}
}
