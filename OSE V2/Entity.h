#pragma once
#include "Component.h"
#include "IDManager.h"

namespace origami_sheep_engine
{
	class Entity
	{
	public:
		Entity(const uint32_t unique_ID, const std::string & name, const std::string & tag, const std::string & prefab);
		~Entity() noexcept;
		Entity(const Entity & other) noexcept;
		Entity & operator=(const Entity & other) noexcept;
		Entity(Entity && other) noexcept;
		Entity & operator=(Entity && other) noexcept;

		const std::string & get_name() const {return this->name_;}
		const uint32_t get_unique_ID() const {return this->unique_ID_;}

		void set_name(const std::string & name) {this->name_ = name;}	//should only be called once named is confirmed as "unique" (see name_ definition)
		void set_tag(const std::string & tag) { this->tag_ = tag; }

		std::vector<Entity> & get_sub_entities() { return this->sub_entities_; }

		std::vector<Component> & get_components() { return this->components_; }

	private:
		std::string name_;		//name_ should be unique to a scene file
		uint32_t unique_ID_;	//unique_ID_ should be unique to a game engine execution

		std::string tag_;		//the lowest level tag applied to this entity (or "")
		std::string prefab_;	//the name of the prefab this entity inherits from (or "")

		std::vector<Entity> sub_entities_;

		std::vector<Component> components_;

		//isVisible, isEnabled, ...
	};
}

