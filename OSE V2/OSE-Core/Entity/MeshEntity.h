#pragma once
#include "../../stdafx.h"
#include "Entity.h"
#include "../Resources/MeshFilter.h"
#include "../Rendering/MeshRenderer.h"

namespace ose::entity
{
	using namespace rendering;
	using namespace resources;

	class MeshEntity : public Entity
	{
	public:
		MeshEntity(const uint32_t unique_ID, const std::string & name, const std::string & tag, const std::string & prefab);
		virtual ~MeshEntity();

		//virtual void addFilter() = 0;
	//	virtual void addRenderer(std::unique_ptr<Renderer> renderer) override;

	private:
		std::unique_ptr<MeshFilter> filter_;
		std::unique_ptr<MeshRenderer> renderer_;
	};
}
