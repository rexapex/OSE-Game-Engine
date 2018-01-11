#pragma once

namespace origami_sheep_engine
{
	class RenderingEngine
	{
	public:
		RenderingEngine();
		virtual ~RenderingEngine();

		virtual void render() = 0;

		virtual void switchTo2DMode(const float left, const float right, const float bottom, const float top) = 0;
		virtual void switchTo3DMode(const float fovyDeg, const float aspect_ratio, const float znear, const float zfar) = 0;
	};
}
