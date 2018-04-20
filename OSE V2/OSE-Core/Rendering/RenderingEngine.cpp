#include "../../stdafx.h"
#include "RenderingEngine.h"

namespace ose::rendering
{
	RenderingEngine::RenderingEngine() : projection_mode_(EProjectionMode::PERSPECTIVE)
	{

	}

	RenderingEngine::~RenderingEngine()
	{

	}

	//saves having to resize framebuffers twice
	void RenderingEngine::set_projection_mode_and_fbsize(const EProjectionMode & projection_mode, const int width, const int height)
	{
		if(this->projection_mode_ != projection_mode)
		{
			this->projection_mode_ = projection_mode;
		}
		this->fbwidth_ = width;
		this->fbheight_ = height;
		this->updateProjectionMatrix();
	}

	void RenderingEngine::set_projection_mode(const EProjectionMode & projection_mode)
	{
		if(this->projection_mode_ != projection_mode)
		{
			this->projection_mode_ = projection_mode;
			this->updateProjectionMatrix();
		}
	}

	void RenderingEngine::set_framebuffer_size(const int width, const int height)
	{
		this->fbwidth_ = width;
		this->fbheight_ = height;
		this->updateProjectionMatrix();
	}

	void RenderingEngine::updateProjectionMatrix()
	{
		if(fbwidth_ != 0 && fbheight_ != 0)		//minimisation and tabbing out need not cause a frame buffer update
		{
			switch(projection_mode_)
			{
			case EProjectionMode::ORTHOGRAPHIC:
			{
				this->updateOrthographicProjectionMatrix(fbwidth_, fbheight_);
				break;
			}
			case EProjectionMode::PERSPECTIVE:
			{
				this->updatePerspectiveProjectionMatrix(90.0f, fbwidth_, fbheight_, 0.0f, 100.0f);
				break;
			}
			}
		}
	}
}
