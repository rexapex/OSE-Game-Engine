#pragma once
#include "OSE-Core/Rendering/EBlendMode.h"

namespace ose
{
	class Texture;
	class ShaderProg;

	class Material
	{
	public:
		Material(std::string const & path, std::string const & name) : path_(path), name_(name) {}
		~Material() {}

		void AddTexture(Texture const * texture)
		{
			// NOTE - Don't check for nullptr since it's important texture indices are preserved whether textures are present or not
			textures_.push_back(texture);
		}

		void SetShaderProg(ShaderProg const * shader_prog)
		{
			shader_prog_ = shader_prog;
		}

		std::string const & GetPath() const { return path_; }
		std::string const & GetName() const { return name_; }

		void SetBlendMode(EBlendMode mode) { blend_mode_ = mode; }
		EBlendMode GetBlendMode() const { return blend_mode_; }

		std::vector<Texture const *> const & GetTextures() const { return textures_; }

		ShaderProg const * GetShaderProg() const { return shader_prog_; }

	private:

		std::string path_;
		std::string name_;

		EBlendMode blend_mode_ { EBlendMode::OPAQUE };

		std::vector<Texture const *> textures_;

		ShaderProg const * shader_prog_ { nullptr };

	public:
		static uptr<Material> NewDefaultOpaqueSpriteMaterial();
		static uptr<Material> NewDefaultAlphaSpriteMaterial();
		static uptr<Material> NewDefaultOpaqueMeshMaterial();
	};
}
