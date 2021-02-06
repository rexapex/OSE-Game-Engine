#pragma once

#include "OSE-Core/Rendering/RenderingEngine.h"
#include "OSE-Core/EngineDependencies/glm/glm.hpp"
#include "RenderPoolVK.h"

namespace ose
{
	class Camera;
}

namespace ose::rendering
{
	struct RenderingEngineVKInternal;

	class RenderingEngineVK final : public RenderingEngine
	{
	public:
		RenderingEngineVK(int fbwidth, int fbheight);
		~RenderingEngineVK();

		// Render one frame to the screen
		void Render(Camera const & active_camera) override;

		// Get a reference to the render pool, s.t. new render objects can be added
		RenderPool & GetRenderPool() override { return render_pool_; }

	private:
		// Load Vulkan functions
		// Return of 0 = success, return of -1 = error
		int InitVulkan();

		// Structure containing internal vulkan data not to be exposed by this header file
		uptr<RenderingEngineVKInternal> internal_;

		// The projection matrix, can be a perspective or an orthographic projection matrix
		glm::mat4 projection_matrix_;

		// The pool of objects rendered each engine update
		RenderPoolVK render_pool_;
		
		// Child functions to update the projection matrix to either orthographic or perspective
		void UpdateOrthographicProjectionMatrix(int fbwidth, int fbheight) override;
		void UpdatePerspectiveProjectionMatrix(float hfov_deg, int fbwidth, int fbheight, float znear, float zfar) override;
	};
}

