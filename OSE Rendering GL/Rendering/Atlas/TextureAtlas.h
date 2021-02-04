#pragma once
#include "TextureAtlasSlot.h"
#include "OccupiedTextureAtlasSlot.h"
#include "../Texture/Texture2DGL.h"

namespace ose
{
	class Texture;
}

namespace ose::rendering
{
	class TextureAtlas final
	{
	public:
		TextureAtlas(int32_t width, int32_t height, int32_t channels);
		~TextureAtlas();

		// Try to add all the textures in the list
		// If any of the textures cannot be added, then none are
		bool TryAddTextures(std::vector<Texture const &> const & textures);

		// Remove a texture from the atlas
		// Returns true iff the texture is removed successfully
		bool RemoveTexture(Texture const & tex);

	private:
		// Add a texture to the atlas
		// Returns true iff the texture is added successfully
		bool AddTexture(Texture const & tex, TextureAtlasSlot * slot);

		// Compute the best fit slot for the given texture dimensions
		TextureAtlasSlot * ComputeBestFitSlot(int32_t w, int32_t h);

		// Create the OpenGL texture
		void Create();

		// Destroy the OpenGL texture
		void Destroy();

	private:
		// OpenGL 2D texture object
		// Wrapper around OpenGL texture ID
		Texture2DGL tex_;

		// Vector of available slot for textures to be added to
		// Slots can be broken down into smaller slots
		// Initially vector contains one slot (0, 0, width, height)
		std::vector<TextureAtlasSlot> available_slots_;

		// Vector of occupied slots (slots containing textures)
		std::vector<OccupiedTextureAtlasSlot> occupied_slots_;

		// Width and height of the atlas (determined by atlas settings)
		int32_t width_;
		int32_t height_;

		// Number of channels, 3 for RGB, 4 for RGBA
		int32_t channels_;
	};
}
