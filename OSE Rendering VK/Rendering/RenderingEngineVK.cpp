#include "pch.h"
#include "RenderingEngineVK.h"
#include "OSE-Core/Game/Camera/Camera.h"
#include "Setup/InstanceVK.h"
#include "OSE-Core/Windowing/WindowManager.h"

namespace ose::rendering
{
	RenderingEngineVK::RenderingEngineVK(WindowManager const & window_manager) : RenderingEngine(window_manager.GetFramebufferWidth(), window_manager.GetFramebufferHeight())
	{
		// NOTE - If RenderingEngineVK is made multithreadable, may need to move this
		if(InitVulkan(window_manager) == -1)
			throw std::exception("Failed to initialise Vulkan");

		// Initialise the render pool only once Vulkan has been intialised
		render_pool_.Init(window_manager.GetFramebufferWidth(), window_manager.GetFramebufferHeight());
		UpdateProjectionMatrix();

		//// Set the default OpenGL settings
		//glCullFace(GL_BACK);
		//glEnable(GL_CULL_FACE);
		//glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		//glEnable(GL_BLEND);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//glEnable(GL_DEPTH_TEST);			// TODO - Disable depth test when rendering a deferred texture (make apart of render pass state)
		//glDepthFunc(GL_LEQUAL);
	}

	RenderingEngineVK::~RenderingEngineVK() {}

	void RenderingEngineVK::UpdateOrthographicProjectionMatrix(int fbwidth, int fbheight)
	{
		DEBUG_LOG("updating othographic projection matrix");
		float aspect_ratio = (float)fbwidth/(float)fbheight;
		// setting glOrtho and glViewport in the following ways worked in testing
		///projection_matrix_ = glm::ortho(-(float)fbwidth/2 * aspect_ratio, (float)fbwidth/2 * aspect_ratio, -(float)fbheight/2 * aspect_ratio, (float)fbheight/2 * aspect_ratio);
		projection_matrix_ = glm::ortho(0.0f, (float)fbwidth, 0.0f, (float)fbheight);

	//	glViewport(0, 0, fbwidth, fbheight);
	}

	void RenderingEngineVK::UpdatePerspectiveProjectionMatrix(float hfov_deg, int fbwidth, int fbheight, float znear, float zfar)
	{
		DEBUG_LOG("updating perspective projection matrix");
		// hfov = 2 * atan(tan(vfov * 0.5) * aspect_ratio)
		// vfov = atan(tan(hfov / 2) / aspect_ratio) * 2
		float aspect_ratio { (float)fbwidth/(float)fbheight };
		float hfov { glm::radians(hfov_deg) };
		float vfov { glm::atan(glm::tan(hfov / 2) / aspect_ratio) * 2 };
		// TODO - test aspect ratio is correct for a variety of resolutions
		projection_matrix_ = glm::perspective(vfov, aspect_ratio, znear, zfar);

	//	glViewport(0, 0, fbwidth, fbheight);	// still required with shaders as far as I'm aware
	}

	// Render one frame to the screen
	void RenderingEngineVK::Render(Camera const & active_camera)
	{
		
	}

	// Load Vulkan functions
	// Return of 0 = success, return of -1 = error
	int RenderingEngineVK::InitVulkan(WindowManager const & window_manager)
	{
		try
		{
			instance_ = ose::make_unique<InstanceVK>(window_manager);
		}
		catch(std::exception & e)
		{
			LOG_ERROR(e.what());
			return -1;
		}
		return 0;
	}
}
