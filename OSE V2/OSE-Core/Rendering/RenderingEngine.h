#pragma once

#include "OSE-Core/EngineReferences.h"
#include "EProjectionMode.h"
#include "OSE-Core/Engine/Engine.h"
#include "OSE-Core/Entity/Entity.h"
#include "OSE-Core/Entity/SpriteRenderer.h"

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

		// called every game update for every data object in pool
		// IMPORTANT - stub method should not be called...
		// ...used to allow update method with RenderObject sub-class to be called
	//	virtual void update(RenderObject &) {}

		// saves having to resize framebuffers twice
		void SetProjectionModeAndFbSize(const EProjectionMode & projection_mode, const int width, const int height);

		void SetProjectionMode(const EProjectionMode & projection_mode);

		void SetFramebufferSize(const int width, const int height);

	private:
		// how the scene will be projected, e.g. ORTHOGRAPHIC, PERSPECTIVE
		EProjectionMode projection_mode_;

		// width and height of the window framebuffer
		int fbwidth_, fbheight_;

		// update the projection matrix based on the projection mode
		void UpdateProjectionMatrix();

		// child functions to update the projection matrix to either orthographic or perspective
		virtual void UpdateOrthographicProjectionMatrix(const int fbwidth, const int fbheight) = 0;
		virtual void UpdatePerspectiveProjectionMatrix(const float fovyDeg, const int fbwidth, const int fbheight, const float znear, const float zfar) = 0;
	};
}
