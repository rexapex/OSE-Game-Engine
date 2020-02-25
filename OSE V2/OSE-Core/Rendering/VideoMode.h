#pragma once

namespace ose
{
	struct VideoMode
	{
		const int width;
		const int height;
		const int refresh_rate;

		VideoMode(const int width, const int height, const int refresh_rate) : width(width), height(height), refresh_rate(refresh_rate) {}
	};
}
