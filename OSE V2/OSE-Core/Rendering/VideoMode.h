#pragma once

namespace ose
{
	struct VideoMode
	{
		int const width;
		int const height;
		int const refresh_rate;

		VideoMode(int width, int height, int refresh_rate) : width(width), height(height), refresh_rate(refresh_rate) {}
	};
}
