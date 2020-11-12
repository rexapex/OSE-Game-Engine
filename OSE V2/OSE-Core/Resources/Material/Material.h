#pragma once

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

		std::vector<Texture const *> const & GetTextures() const { return textures_; }

		ShaderProg const * GetShaderProg() const { return shader_prog_; }

	private:

		std::string path_;
		std::string name_;

		std::vector<Texture const *> textures_;

		ShaderProg const * shader_prog_;
	};
}
