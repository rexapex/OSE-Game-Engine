#include "stdafx.h"
#include "RenderingEngine.h"
#include "OSE-Core/Project/ProjectSettings.h"

namespace ose
{
	RenderingEngine::RenderingEngine(int fbwidth, int fbheight) : projection_mode_(EProjectionMode::PERSPECTIVE), fbwidth_(fbwidth), fbheight_(fbheight)
	{

	}

	RenderingEngine::~RenderingEngine()
	{

	}

	// Apply rendering settings to the rendering engine
	void RenderingEngine::ApplyRenderingSettings(RenderingSettings const & rendering_settings)
	{
		projection_mode_ = rendering_settings.projection_mode_;
		znear_ = rendering_settings.znear_;
		zfar_ = rendering_settings.zfar_;
		UpdateProjectionMatrix();
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
		this->GetRenderPool().SetFramebufferSize(width, height);
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
				this->UpdatePerspectiveProjectionMatrix(90.0f, fbwidth_, fbheight_, znear_, zfar_);
				break;
			}
			}
		}
	}
}