#include "pch.h"
#include "TextureAtlas.h"
#include "OSE-Core/Resources/Texture/Texture.h"

namespace ose::rendering
{
	TextureAtlas::TextureAtlas(int32_t width, int32_t height, int32_t channels) : width_(width), height_(height), channels_(channels)
	{
		available_slots_.emplace_back(0, 0, width, height);
	}

	TextureAtlas::~TextureAtlas()
	{
	
	}

	// Try to add all the textures in the list
	// If any of the textures cannot be added, then none are
	bool TextureAtlas::TryAddTextures(std::vector<Texture const &> const & textures)
	{
		for(auto const & texture : textures)
		{
			// First, check if the texture already exists in this atlas
			bool found { false };
			for(auto const & slot : occupied_slots_)
			{
				if(&slot.texture_ == &texture)
					found = true;
			}

			// If the texture does not exist in the atlas, see if there is space for it
			if(!found)
			{
				TextureAtlasSlot * slot { ComputeBestFitSlot(texture.GetWidth(), texture.GetHeight()) };

				// IMPORTANT TODO - This only works if textures contains one texture !!!!!!!!
				return AddTexture(texture, slot);
			}
		}
	}

	// Add a texture to the atlas
	// Returns true iff the texture is added successfully
	bool TextureAtlas::AddTexture(Texture const & tex, TextureAtlasSlot * slot)
	{
		// Find the smallest slot which fits the texture
		int32_t w = tex.GetWidth();
		int32_t h = tex.GetHeight();

		// Update available slots
		if(slot)
		{
			TextureAtlasSlot tex_slot(slot->x_, slot->y_, w, h);
			int32_t unused_w = slot->w_ - w;
			int32_t unused_h = slot->h_ - h;
			available_slots_.emplace_back(tex_slot.x_+tex_slot.w_, tex_slot.y_, unused_w, slot->h_);
			available_slots_.emplace_back(tex_slot.x_, tex_slot.y_+tex_slot.h_, slot->w_-unused_w, unused_h);
			available_slots_.erase(std::remove_if(available_slots_.begin(), available_slots_.end(), [slot](auto & slot) { return best_fit_slot == &slot; }), available_slots_.end());
			
			// Upload data to texture2d
			tex_.UploadData(tex_slot.x_, tex_slot.y_, w, h, tex.GetNumChannels(), tex.GetImgData());

			return true;
		}

		return false;
	}

	// Remove a texture from the atlas
	// Returns true iff the texture is removed successfully
	bool TextureAtlas::RemoveTexture(Texture const & tex)
	{
		// Find and remove the slot
		// Re-add the slot to the available slots list
		// Merge with nearby slots if required
		return false;
	}

	// Compute the best fit slot for the given texture dimensions
	TextureAtlasSlot * TextureAtlas::ComputeBestFitSlot(int32_t w, int32_t h)
	{
		int32_t a = w * h;
		int32_t closeness = -1;
		TextureAtlasSlot * best_fit_slot { nullptr };
		for(auto & slot : available_slots_)
		{
			if(slot.w_ >= w && slot.h_ >= h)
			{
				int32_t slot_area = slot.w_ * slot.h_;
				int32_t c = slot_area - a;
				closeness = std::min(c, closeness);
				if(closeness == c)
				{
					best_fit_slot = &slot;
				}
			}
		}
	}

	// Create the OpenGL texture
	void TextureAtlas::Create()
	{
		tex_.Create(width_, height_, channels_, nullptr);
	}

	// Destroy the OpenGL texture
	void TextureAtlas::Destroy()
	{
		tex_.Destroy();
	}
}
