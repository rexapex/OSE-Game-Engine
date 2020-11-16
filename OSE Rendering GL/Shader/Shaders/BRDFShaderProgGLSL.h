#pragma once
#include "../ShaderProgGLSL.h"

namespace ose::shader
{
	class BRDFShaderProgGLSL final : public ShaderProgGLSL
	{
	public:
		BRDFShaderProgGLSL();
		virtual ~BRDFShaderProgGLSL();

		// Build an OpenGL shader object from a shader graph
		void CreateShaderProg() override;

		// Destroy the OpenGL shader object
		void DestroyShaderProg() override;
	};
}
