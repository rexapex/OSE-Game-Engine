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
		void AddSpriteRenderer(math::ITransform const & t, ose::unowned_ptr<entity::SpriteRenderer> sr);

		// Add a tile renderer component to the render pool
		void AddTileRenderer(ose::math::ITransform const & t, ose::unowned_ptr<ose::entity::TileRenderer> tr);

		// Get the list of render passes s.t. they can be rendered by the rendering engine
		std::vector<RenderPassGL> const & GetRenderPasses() const { return render_passes_; }

	private:
		// List of all render passes the render pool is to perform on each rendering engine update
		std::vector<RenderPassGL> render_passes_;
	};
}

