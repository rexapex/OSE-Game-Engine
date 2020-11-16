#pragma once
#include "../ShaderProgGLSL.h"

namespace ose::shader
{
	class Default3DShaderProgGLSL final : public ShaderProgGLSL
	{
	public:
		Default3DShaderProgGLSL();
		virtual ~Default3DShaderProgGLSL();

		// Build an OpenGL shader object from a shader graph
		void CreateShaderProg() override;

		// Destroy the OpenGL shader object
		void DestroyShaderProg() override;
	};
}
