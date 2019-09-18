#include "stdafx.h"
#include "NConstColor.h"

namespace ose::shader
{
	NConstColor::NConstColor() : ShaderNode()
	{
		outputs_.emplace("color", ShaderVarType{ EBaseVarType::FLOAT });
	}

	NConstColor::NConstColor(glm::vec4 const & color) : ShaderNode(), color_(color)
	{
		outputs_.emplace("color", ShaderVarType{ EBaseVarType::FLOAT });
	}

	NConstColor::~NConstColor()
	{

	}
}
