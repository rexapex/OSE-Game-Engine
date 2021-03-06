#include "stdafx.h"
#include "Tilemap.h"

namespace ose
{
	Tilemap::Tilemap(std::string const & name, std::string const & path) : name_(name), path_(path),
		width_(0), height_(0), tile_grid_(nullptr)
	{

	}
	
	Tilemap::~Tilemap()
	{
		delete[] tile_grid_;
	}

	Tilemap::Tilemap(Tilemap && other) noexcept
		: name_(""), path_(""), width_(0), height_(0),
		tile_grid_(nullptr)
	{
		std::swap(*this, other);
	}

	Tilemap & Tilemap::operator=(Tilemap && other) noexcept
	{
		name_ = "";
		path_ = "";
		width_ = 0;
		height_ = 0;
		tile_grid_ = nullptr;
		std::swap(*this, other);
		return *this;
	}

	void Tilemap::CreateTileGrid(int32_t width, int32_t height) noexcept
	{
		if(width > 0 && height > 0)
		{
			delete[] tile_grid_;
			width_ = width;
			height_ = height;
			try
			{
				tile_grid_ = new int32_t[static_cast<size_t>(width) * height];
			}
			catch(std::bad_alloc &)
			{
				width_ = 0;
				height_ = 0;
				tile_grid_ = nullptr;
			}
		}
	}

	void Tilemap::DeleteTileGrid() noexcept
	{
		width_ = 0;
		height_ = 0;
		delete[] tile_grid_;
		tile_grid_ = nullptr;
	}
}
