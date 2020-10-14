#include "stdafx.h"
#include "Scene.h"
#include "OSE-Core/Entity/Entity.h"
#include "SceneManager.h"

namespace ose
{
	Scene::Scene(const std::string & name, const ControlSettings & control_settings)
		: EntityList(), ChunkManager(), name_(name), control_settings_(control_settings) {}

	Scene::~Scene() noexcept {}

	Scene::Scene(const Scene & other) noexcept : EntityList(other), ChunkManager(other)
	{
		this->name_ = other.name_;
		this->control_settings_ = other.control_settings_;
	}

	void Scene::OnChunkActivated(Chunk & chunk)
	{
		scene_manager_->OnChunkActivated(chunk);
	}

	void Scene::OnChunkDeactivated(Chunk & chunk)
	{
		scene_manager_->OnChunkDeactivated(chunk);
	}



	// DEBUG METHODS
	// TODO - REMOVE WHEN READY
	void Scene::Print()
	{
		std::cerr << "**********  Scene Definition  **********" << std::endl;
		std::cerr << "scene -> name: " << this->name_ << std::endl;
		for(auto & e : entities_)
		{
			PrintEntity(*e, 1);
		}
	}


	void Scene::PrintEntity(const Entity & e, int32_t level)
	{
		std::string indent = "";
		for(int32_t i = 0; i < level; i++)
		{
			indent += "    ";
		}

		std::cerr << indent << "entity -> id: " << e.GetUniqueId() << ", name: " << e.GetName() << std::endl;

		/*for(auto & comp : e.get_components())
		{
			std::cerr << indent << "    comp -> " << comp->name << std::endl;
		}*/

		for(const auto & subE : e.GetEntities())
		{
			PrintEntity(*subE, level + 1);
		}
	}
}
