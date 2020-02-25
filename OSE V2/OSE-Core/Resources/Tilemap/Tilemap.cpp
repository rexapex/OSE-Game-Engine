#include "stdafx.h"
#include "Tilemap.h"

namespace ose
{
	Tilemap::Tilemap(const std::string & name, const std::string & path) : name_(name), path_(path),
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

	void Tilemap::DeleteTileGrid() noexcept
	{
		width_ = 0;
		height_ = 0;
		auto ptr = tile_grid_;
		tile_grid_ = nullptr;
		delete[] ptr;
	}
}
