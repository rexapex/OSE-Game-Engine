#pragma once

namespace ose
{
	class Texture;

	class Material
	{
	public:
		Material(std::string const & path, std::string const & name) : path_(path), name_(name) {}
		~Material() {}

		void AddTexture(unowned_ptr<Texture const> texture)
		{
			// NOTE - Don't check for nullptr since it's important texture indices are preserved whether textures are present or not
			textures_.push_back(texture);
		}

		std::string const & GetPath() const { return path_; }
		std::string const & GetName() const { return name_; }

		std::vector<unowned_ptr<Texture const>> const & GetTextures() const { return textures_; }

	private:

		std::string path_;
		std::string name_;

		std::vector<unowned_ptr<Texture const>> textures_;
	};
}
