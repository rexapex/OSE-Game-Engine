#include "stdafx.h"
#include "Tilemap.h"

namespace ose::resources
{
	Tilemap::Tilemap(const std::string & name, const std::string & path, int32_t width, int32_t height) : name_(name), path_(path),
		width_(width), height_(height), spacing_x_(0), spacing_y_(0), tile_grid_(nullptr)
	{
		if(width_ <= 0)
			width_ = 1;
		if(height_ <= 0)
			height_ = 1;
		tile_grid_ = new int32_t[static_cast<size_t>(width_) * height_];
	}
	
	Tilemap::~Tilemap()
	{
		delete[] tile_grid_;
	}

	Tilemap::Tilemap(Tilemap && other) noexcept
		: name_(""), path_(""), width_(1), height_(1),
		spacing_x_(0), spacing_y_(0), tile_grid_(nullptr)
	{
		std::swap(*this, other);
	}

	Tilemap & Tilemap::operator=(Tilemap && other) noexcept
	{
		name_ = "";
		path_ = "";
		width_ = 1;
		height_ = 1;
		spacing_x_ = 0;
		spacing_y_ = 0;
		tile_grid_ = nullptr;
		std::swap(*this, other);
	}
}
