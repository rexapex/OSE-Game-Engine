#pragma once
#include "Component.h"
#include "IDManager.h"
#include "Transform.h"

namespace origami_sheep_engine
{
	class Entity
	{
	public:
		Entity(const uint32_t unique_ID, const std::string & name, const std::string & tag, const std::string & prefab);
		~Entity() noexcept;
		Entity(const Entity & other) noexcept;
		Entity & operator=(const Entity & other) noexcept;
		Entity(Entity && other) noexcept = default;
		Entity & operator=(Entity && other) noexcept = default;

		const std::string & get_name() const {return this->name_;}
		const uint32_t get_unique_ID() const {return this->unique_ID_;}

		void set_name(const std::string & name) {this->name_ = name;}	//should only be called once named is confirmed as "unique" (see name_ definition)
		void set_tag(const std::string & tag) { this->tag_ = tag; }

		std::vector<Entity> & get_sub_entities() { return this->sub_entities_; }

		std::vector<std::unique_ptr<Component>> & get_components() { return this->components_; }

		//read-only transform relative the parent entity/world if no parent exists
		const Transform & get_local_transform() const { return this->local_transform_; }

		//read-only transform relative to the world
		const Transform & get_global_transform() const { return this->global_transform_; }

		//modify the local and global transform of the entity
		void translate(const glm::vec3 & translation);
		void translate(const float x, const float y, const float z);

		//rotate by radians
		void rotate(const glm::vec3 & change);
		void rotate(const float pitch, const float yaw, const float roll);
		//rotate by degrees
		void rotateDeg(const glm::vec3 & change);
		void rotateDeg(const float pitch, const float yaw, const float roll);

		void scale(const float scalar);
		void scale(const glm::vec3 & multiplier);
		void scale(const float x, const float y, const float z);

		//modify just the global transform of the entity
		void translateParent(const glm::vec3 & translation);
		void translateParent(const float x, const float y, const float z);

		//rotate by radians
		void rotateParent(const glm::vec3 & change);
		void rotateParent(const float pitch, const float yaw, const float roll);
		//rotate by degrees
		void rotateDegParent(const glm::vec3 & change);
		void rotateDegParent(const float pitch, const float yaw, const float roll);

		void scaleParent(const float scalar);
		void scaleParent(const glm::vec3 & multiplier);
		void scaleParent(const float x, const float y, const float z);

	private:
		std::string name_;		//name_ should be unique to a scene file
		uint32_t unique_ID_;	//unique_ID_ should be unique to a game engine execution

		std::string tag_;		//the lowest level tag applied to this entity (or "")
		std::string prefab_;	//the name of the prefab this entity inherits from (or "")

		std::vector<Entity> sub_entities_;

		std::vector<std::unique_ptr<Component>> components_;

		Transform local_transform_;		//the transform of the entity relative to the parent
		Transform global_transform_;	//the transform of the entity relative to the world

		//isVisible, isEnabled, ...
	};
}

