#include "stdafx.h"
#include "RenderingEngine.h"

namespace ose
{
	RenderingEngine::RenderingEngine() : projection_mode_(EProjectionMode::PERSPECTIVE), fbwidth_(0), fbheight_(0)
	{

	}

	RenderingEngine::~RenderingEngine()
	{

	}

	//saves having to resize framebuffers twice
	void RenderingEngine::SetProjectionModeAndFbSize(const EProjectionMode & projection_mode, const int width, const int height)
	{
		if(this->projection_mode_ != projection_mode)
		{
			this->projection_mode_ = projection_mode;
		}
		this->fbwidth_ = width;
		this->fbheight_ = height;
		this->UpdateProjectionMatrix();
	}

	void RenderingEngine::SetProjectionMode(const EProjectionMode & projection_mode)
	{
		if(this->projection_mode_ != projection_mode)
		{
			this->projection_mode_ = projection_mode;
			this->UpdateProjectionMatrix();
		}
	}

	void RenderingEngine::SetFramebufferSize(const int width, const int height)
	{
		this->fbwidth_ = width;
		this->fbheight_ = height;
		this->UpdateProjectionMatrix();
	}

	void RenderingEngine::UpdateProjectionMatrix()
	{
		if(fbwidth_ != 0 && fbheight_ != 0)		//minimisation and tabbing out need not cause a frame buffer update
		{
			switch(projection_mode_)
			{
			case EProjectionMode::ORTHOGRAPHIC:
			{
				this->UpdateOrthographicProjectionMatrix(fbwidth_, fbheight_);
				break;
			}
			case EProjectionMode::PERSPECTIVE:
			{
				this->UpdatePerspectiveProjectionMatrix(90.0f, fbwidth_, fbheight_, 0.0f, 100.0f);
				break;
			}
			}
		}
	}
}