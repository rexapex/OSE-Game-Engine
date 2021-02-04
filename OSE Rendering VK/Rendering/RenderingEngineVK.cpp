#include "pch.h"
#include "RenderingEngineVK.h"
#include "OSE-Core/Game/Camera/Camera.h"

namespace ose::rendering
{
	RenderingEngineVK::RenderingEngineVK(int fbwidth, int fbheight) : RenderingEngine(fbwidth, fbheight)
	{
		//// NOTE - If RenderingEngineGL is made multithreadable, may need to move this
		//// TODO - Only load GLEW if used OpenGL functions are not available
		//InitGlew();

		// Initialise the render pool only once OpenGL has been intialised
		//render_pool_.Init(fbwidth, fbheight);
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

	//// Load OpenGL functions using GLEW
	//// Return of 0 = success, return of -1 = error
	//int RenderingEngineVK::InitGlew()
	//{
	//	GLenum err = glewInit();
	//	if(GLEW_OK != err)
	//	{
	//		// GLEW Initialisation failed
	//		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
	//		return -1;		// return with error
	//	}
	//	fprintf(stderr, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));

	//	std::cerr << "Initialised GLEW" << std::endl;
	//	if(GLEW_VERSION_3_2)
	//	{
	//		std::cerr << "OpenGL 3.2 Supported" << std::endl;
	//	}
	//	else
	//	{
	//		std::cerr << "OpenGL 3.2 Not Supported" << std::endl;

	//		if(GLEW_ARB_instanced_arrays)
	//		{
	//			std::cerr << "Instanced Arrays Available" << std::endl;
	//		}
	//		else
	//		{
	//			std::cerr << "Instanced Arrays Not Supported" << std::endl;
	//		}
	//	}

	//	return 0;			// return with success
	//}
}
