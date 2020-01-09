#pragma once
#include "OSE-Core/Rendering/RenderPool.h"
#include "OSE-Core/Math/ITransform.h"
#include "RenderPassGL.h"

namespace ose::rendering
{
	class RenderPoolGL : public RenderPool
	{
	public:
		RenderPoolGL();
		~RenderPoolGL();

		// Initialise the render pool
		void Init();

		// Add a sprite renderer component to the render pool
		void AddSpriteRenderer(ose::ITransform const & t, unowned_ptr<SpriteRenderer> sr);

		// Add a tile renderer component to the render pool
		void AddTileRenderer(ose::ITransform const & t, unowned_ptr<TileRenderer> tr);

		// Remove a sprite renderer component from the render pool
		void RemoveSpriteRenderer(unowned_ptr<SpriteRenderer> sr) override;

		// Remove a tile renderer component from the render pool
		void RemoveTileRenderer(unowned_ptr<TileRenderer> tr) override;

		// Get the list of render passes s.t. they can be rendered by the rendering engine
		std::vector<RenderPassGL> const & GetRenderPasses() const { return render_passes_; }

	private:
		// List of all render passes the render pool is to perform on each rendering engine update
		std::vector<RenderPassGL> render_passes_;

		// Returns the next available object id
		static uint32_t NextComponentId()
		{
			static uint32_t id { 0 };
			return id++;
			// TODO - Test whether out of ids, if ran out should reset all ids and notify entity components
			// TODO - Alternatively use 64-bit integer which is unlikely to ever reach max value (typedef id)
		}
	};
}

