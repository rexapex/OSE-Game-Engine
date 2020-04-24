#include "pch.h"
#include "RenderingEngineGL.h"

namespace ose::rendering
{
	RenderingEngineGL::RenderingEngineGL(int fbwidth, int fbheight) : RenderingEngine(fbwidth, fbheight)
	{
		// NOTE - If RenderingEngineGL is made multithreadable, may need to move this
		// TODO - Only load GLEW if used OpenGL functions are not available
		InitGlew();

		// Initialise the render pool only once OpenGL has been intialised
		render_pool_.Init(fbwidth, fbheight);
		UpdateProjectionMatrix();

		// Set the default OpenGL settings
		glCullFace(GL_BACK);
		glEnable(GL_CULL_FACE);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_DEPTH_TEST);			// TODO - Disable depth test when rendering a deferred texture (make apart of render pass state)
		glDepthFunc(GL_LESS);
	}

	RenderingEngineGL::~RenderingEngineGL() {}

	void RenderingEngineGL::UpdateOrthographicProjectionMatrix(const int fbwidth, const int fbheight)
	{
		DEBUG_LOG("updating othographic projection matrix");
		float aspect_ratio = (float)fbwidth/(float)fbheight;
		// setting glOrtho and glViewport in the following ways worked in testing
		///projection_matrix_ = glm::ortho(-(float)fbwidth/2 * aspect_ratio, (float)fbwidth/2 * aspect_ratio, -(float)fbheight/2 * aspect_ratio, (float)fbheight/2 * aspect_ratio);
		projection_matrix_ = glm::ortho(0.0f, (float)fbwidth, 0.0f, (float)fbheight);
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
			// Bind the fbo and clear the required buffers
			glBindFramebuffer(GL_FRAMEBUFFER, render_pass.fbo_);
			glClear(render_pass.clear_);

			for(auto const & shader_group : render_pass.shader_groups_)
			{
				// Bind the shader used by the shader group
				glUseProgram(shader_group.shader_prog_);

				// Pass the lights to the shader program
				glUniform1i(glGetUniformLocation(shader_group.shader_prog_, "numPointLights"), std::min(static_cast<int>(render_pool_.GetPointLights().size()), 16));
				for(size_t l = 0; l < render_pool_.GetPointLights().size() && l < 16; l++)
				{
					auto const & light = render_pool_.GetPointLights()[l];
					glUniform4f(glGetUniformLocation(shader_group.shader_prog_, std::string{"pointLights[" + std::to_string(l) + "].position"}.c_str()), light.position_.x, light.position_.y, light.position_.z, light.position_.w);
					glUniform4f(glGetUniformLocation(shader_group.shader_prog_, std::string{"pointLights[" + std::to_string(l) + "].color"}.c_str()), light.color_.x, light.color_.y, light.color_.z, light.color_.w);
					glUniform1f(glGetUniformLocation(shader_group.shader_prog_, std::string{"pointLights[" + std::to_string(l) + "].intensity"}.c_str()), light.intensity_);
				}

				// Pass the view projection matrix to the shader program
				glm::mat4 camera = glm::lookAt(glm::vec3{ 0, 0, -10}, glm::vec3{0, 0, 0}, glm::vec3{0, 1, 0});
				glm::mat4 view_proj = projection_matrix_ * camera;
				glUniformMatrix4fv(glGetUniformLocation(shader_group.shader_prog_, "viewProjMatrix"), 1, GL_FALSE, glm::value_ptr(view_proj));

				// Render the render objects one by one
				for(auto const & render_object : shader_group.render_objects_)
				{
					for(size_t i = 0; i < render_object.transforms_.size(); ++i)
					{
						// Pass the world transform of the object to the shader program
						glm::mat4 tMat { render_object.transforms_[i]->GetTransformMatrix() };
						glUniformMatrix4fv(glGetUniformLocation(shader_group.shader_prog_, "worldTransform"), 1, GL_FALSE, glm::value_ptr(tMat));

						// Bind the textures
						for(size_t t = 0; t < render_object.texture_stride_; ++t)
						{
							glActiveTexture(GL_TEXTURE0 + t);
							glBindTexture(GL_TEXTURE_2D, render_object.textures_[i * render_object.texture_stride_ + t]);
						}

						// Render the object
						glBindVertexArray(render_object.vao_);
						if(render_object.ibo_ == 0)
							glDrawArrays(render_object.render_primitive_, render_object.first_, render_object.count_);
						else
							glDrawElements(render_object.render_primitive_, render_object.count_, GL_UNSIGNED_INT, 0);
					}
				}
			}
		}
		glBindVertexArray(0);
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
