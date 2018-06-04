#pragma once

#include "EProjectionMode.h"
#include "OSE-Core/Engine/Engine.h"
#include "OSE-Core/Entity/Entity.h"

namespace ose::rendering
{
	using namespace engine;
	using namespace entity;
	using namespace resources;

	class RenderingEngine : public Engine
	{
	public:
		RenderingEngine();
		virtual ~RenderingEngine();

		// NOTE - replaced with update() method from Engine class
		///virtual void render() = 0;

		//saves having to resize framebuffers twice
		void set_projection_mode_and_fbsize(const EProjectionMode & projection_mode, const int width, const int height);

		void set_projection_mode(const EProjectionMode & projection_mode);

		void set_framebuffer_size(const int width, const int height);

		//create a rendering engine specific texture object for a ose texture object
		virtual void createTexture(const Texture & texture) = 0;

		//delete a rendering engine specific texture object
		virtual void deleteTexture(const Texture & texture) = 0;

	private:
		//how the scene will be projected, e.g. ORTHOGRAPHIC, PERSPECTIVE
		EProjectionMode projection_mode_;

		//width and height of the window framebuffer
		int fbwidth_, fbheight_;

		//update the projection matrix based on the projection mode
		void updateProjectionMatrix();

		//child functions to update the projection matrix to either orthographic or perspective
		virtual void updateOrthographicProjectionMatrix(const int fbwidth, const int fbheight) = 0;
		virtual void updatePerspectiveProjectionMatrix(const float fovyDeg, const int fbwidth, const int fbheight, const float znear, const float zfar) = 0;
	};
}
