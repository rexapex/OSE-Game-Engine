#include "pch.h"
#include "RenderingEngineGL.h"

namespace ose::rendering
{
	RenderingEngineGL::RenderingEngineGL() : RenderingEngine()
	{
		// NOTE - If RenderingEngineGL is made multithreadable, may need to move this
		// TODO - Only load GLEW if used OpenGL functions are not available
		InitGlew();

		// Initialise the render pool only once OpenGL has been intialised
		render_pool_.Init();

		// Set the default OpenGL settings
		glDisable(GL_CULL_FACE);
		///glCullFace(GL_BACK);
		///glEnable(GL_CULL_FACE);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	}

	RenderingEngineGL::~RenderingEngineGL() {}

	void RenderingEngineGL::UpdateOrthographicProjectionMatrix(const int fbwidth, const int fbheight)
	{
		DEBUG_LOG("updating othographic projection matrix");
		float aspect_ratio = (float)fbwidth/(float)fbheight;
		// setting glOrtho and glViewport in the following ways worked in testing
		projection_matrix_ = glm::ortho(-(float)fbwidth/2 * aspect_ratio, (float)fbwidth/2 * aspect_ratio, -(float)fbheight/2 * aspect_ratio, (float)fbheight/2 * aspect_ratio);
		glViewport(0, 0, fbwidth, fbheight);
	}

	void RenderingEngineGL::UpdatePerspectiveProjectionMatrix(const float fovyDeg, const int fbwidth, const int fbheight, const float znear, const float zfar)
	{
		DEBUG_LOG("updating perspective projection matrix");
		// TODO - test aspect ratio is correct for a variety of resolutions
		projection_matrix_ = glm::perspective(glm::radians(fovyDeg), (float)fbwidth/(float)fbheight, znear, zfar);
		glViewport(0, 0, fbwidth, fbheight);	// still required with shaders as far as I'm aware
	}

	// Engine::update method overriden
	// Called every game update to render all object in the pool
	void RenderingEngineGL::Update()
	{
		for(auto const & render_pass : render_pool_.GetRenderPasses())
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			for(auto const & shader_group : render_pass.shader_groups_)
			{
				// Bind the shader used by the shader group
				glUseProgram(shader_group.shader_prog_);

				// Pass the view projection matrix to the shader program
				glUniformMatrix4fv(glGetUniformLocation(shader_group.shader_prog_, "viewProjMatrix"), 1, GL_FALSE, glm::value_ptr(projection_matrix_));

				// Render the render objects one by one
				for(auto const & render_object : shader_group.render_objects_)
				{
					// TODO - Allow a single render object to bind multiple textures simultaneously
					// TODO - Could use a vector num_textures_ that gives the number of textures used by object i
					for(size_t i = 0; i < render_object.transforms_.size(); i++)
					{
						// Pass the world transform of the object to the shader program
						glUniformMatrix4fv(glGetUniformLocation(shader_group.shader_prog_, "worldTransform"), 1, GL_FALSE, glm::value_ptr(render_object.transforms_[i]));

						// Bind the texture
						glActiveTexture(GL_TEXTURE0);
						glBindTexture(GL_TEXTURE_2D, render_object.textures_[i]->GetGlTexId());

						// Render the object
						glBindVertexArray(render_object.vao_);
						glBindBuffer(GL_VERTEX_ARRAY, render_object.vbo_);
						glDrawArrays(GL_QUADS, 0, 4);
					}
				}
			}
		}
	}

	// Load OpenGL functions using GLEW
	// Return of 0 = success, return of -1 = error
	int RenderingEngineGL::InitGlew()
	{
		GLenum err = glewInit();
		if(GLEW_OK != err)
		{
			// GLEW Initialisation failed
			fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
			return -1;		// return with error
		}
		fprintf(stderr, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));

		std::cerr << "Initialised GLEW" << std::endl;
		if(GLEW_VERSION_3_2)
		{
			std::cerr << "OpenGL 3.2 Supported" << std::endl;
		}
		else
		{
			std::cerr << "OpenGL 3.2 Not Supported" << std::endl;

			if(GLEW_ARB_instanced_arrays)
			{
				std::cerr << "Instanced Arrays Available" << std::endl;
			}
			else
			{
				std::cerr << "Instanced Arrays Not Supported" << std::endl;
			}
		}

		return 0;			// return with success
	}
}
