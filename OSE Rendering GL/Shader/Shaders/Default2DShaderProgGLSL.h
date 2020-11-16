#pragma once
#include "../ShaderProgGLSL.h"

namespace ose::shader
{
	class Default2DShaderProgGLSL final : public ShaderProgGLSL
	{
	public:
		Default2DShaderProgGLSL();
		virtual ~Default2DShaderProgGLSL();

		// Build an OpenGL shader object from a shader graph
		void CreateShaderProg() override;

		// Destroy the OpenGL shader object
		void DestroyShaderProg() override;
	};
}
