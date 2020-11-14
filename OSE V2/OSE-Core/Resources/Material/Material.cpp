#include "stdafx.h"
#include "Material.h"

namespace ose
{
	uptr<Material> Material::NewDefaultOpaqueSpriteMaterial()
	{
		uptr<Material> mat = ose::make_unique<Material>("", "OSE-DefaultOpaqueSpriteMaterial");
		mat->blend_mode_ = EBlendMode::OPAQUE;
		mat->shader_prog_ = nullptr;
		return std::move(mat);
	}

	uptr<Material> Material::NewDefaultAlphaSpriteMaterial()
	{
		uptr<Material> mat = ose::make_unique<Material>("", "OSE-DefaultAlphaSpriteMaterial");
		mat->blend_mode_ = EBlendMode::ONE_MINUS_SRC_ALPHA;
		mat->shader_prog_ = nullptr;
		return std::move(mat);
	}
}
