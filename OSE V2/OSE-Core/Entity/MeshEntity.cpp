#include "../../stdafx.h"
#include "MeshEntity.h"

namespace ose::entity
{
	MeshEntity::MeshEntity(const uint32_t unique_ID, const std::string & name, const std::string & tag, const std::string & prefab)
			: Entity(unique_ID, name, tag, prefab)
	{
	}

	MeshEntity::~MeshEntity()
	{
	}

//	void MeshEntity::addRenderer(std::unique_ptr<Renderer> renderer)
//	{
		// CANNOT FIND A GOOD SOLUTION TO THIS

	//	auto mesh_renderer = std::move(dynamic_cast<MeshRenderer*>(renderer.get()));
	//	if(renderer)
//	}
}
