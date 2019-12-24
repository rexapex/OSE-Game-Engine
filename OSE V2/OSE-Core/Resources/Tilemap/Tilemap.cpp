#include "stdafx.h"
#include "Tilemap.h"

namespace ose::resources
{
	Tilemap::Tilemap(const std::string & name, const std::string & path) : name_(name), path_(path),
		width_(0), height_(0), spacing_x_(0), spacing_y_(0), tile_grid_(nullptr)
	{

	}
	
	Tilemap::~Tilemap()
	{
		delete[] tile_grid_;
	}

	Tilemap::Tilemap(Tilemap && other) noexcept
		: name_(""), path_(""), width_(0), height_(0),
		spacing_x_(0), spacing_y_(0), tile_grid_(nullptr)
	{
		std::swap(*this, other);
	}

	Tilemap & Tilemap::operator=(Tilemap && other) noexcept
	{
		name_ = "";
		path_ = "";
		width_ = 0;
		height_ = 0;
		spacing_x_ = 0;
		spacing_y_ = 0;
		tile_grid_ = nullptr;
		std::swap(*this, other);
	}

	void Tilemap::SetTileGrid(int32_t width, int32_t height) noexcept
	{
		if(width > 0 && height > 0)
		{
			width_ = width;
			height_ = height;
			delete[] tile_grid_;
			try
			{
				tile_grid_ = new int32_t[static_cast<size_t>(width) * height];
			}
			catch(std::bad_alloc &)
			{
				tile_grid_ = nullptr;
			}
		}
	}
}
