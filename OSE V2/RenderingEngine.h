#pragma once

#include "stdafx.h"
#include "EProjectionMode.h"

namespace origami_sheep_engine
{
	class RenderingEngine
	{
	public:
		RenderingEngine();
		virtual ~RenderingEngine();

		virtual void render() = 0;

		void set_projection_mode(const EProjectionMode & projection_mode);

		void set_framebuffer_size(const int width, const int height);

	private:
		EProjectionMode projection_mode_;

		int fbwidth_, fbheight_;

		void updateProjectionMatrix();

		virtual void updateOrthographicProjectionMatrix(const int fbwidth, const int fbheight) = 0;
		virtual void updatePerspectiveProjectionMatrix(const float fovyDeg, const int fbwidth, const int fbheight, const float znear, const float zfar) = 0;
	};
}
