#pragma once
#include "Shader.h"
#include "Nodes/NConstColor.h"

namespace ose::shader
{
	class ShaderBuilder
	{
	public:

		ShaderBuilder();
		~ShaderBuilder();
		
		void BuildShader(Shader & shader);

	private:

		virtual void BuildConstColorNode(NConstColor & node) = 0;
	};
}

