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
		hfov_deg_ = rendering_settings.hfov_;
		UpdateProjectionMatrix();
	}

	//saves having to resize framebuffers twice
	void RenderingEngine::SetProjectionModeAndFbSize(EProjectionMode const & projection_mode, int width, int height)
	{
		if(projection_mode_ != projection_mode)
		{
			projection_mode_ = projection_mode;
		}
		fbwidth_ = width;
		fbheight_ = height;
		UpdateProjectionMatrix();
	}

	void RenderingEngine::SetProjectionMode(EProjectionMode const & projection_mode)
	{
		if(projection_mode_ != projection_mode)
		{
			projection_mode_ = projection_mode;
			UpdateProjectionMatrix();
		}
	}

	void RenderingEngine::SetFramebufferSize(int width, int height)
	{
		fbwidth_ = width;
		fbheight_ = height;
		GetRenderPool().SetFramebufferSize(width, height);
		UpdateProjectionMatrix();
	}

	void RenderingEngine::UpdateProjectionMatrix()
	{
		if(fbwidth_ != 0 && fbheight_ != 0)		//minimisation and tabbing out need not cause a frame buffer update
		{
			switch(projection_mode_)
			{
			case EProjectionMode::ORTHOGRAPHIC:
			{
				UpdateOrthographicProjectionMatrix(fbwidth_, fbheight_);
				break;
			}
			case EProjectionMode::PERSPECTIVE:
			{
				UpdatePerspectiveProjectionMatrix(hfov_deg_, fbwidth_, fbheight_, znear_, zfar_);
				break;
			}
			}
		}
	}
}