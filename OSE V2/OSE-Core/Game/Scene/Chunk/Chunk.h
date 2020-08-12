#pragma once
#include "OSE-Core/Entity/EntityList.h"
#include "OSE-Core/Math/Transformable.h"

namespace ose
{
	class Game;

	class Chunk : public EntityList, public Transformable<std::unique_ptr<Entity>>
	{
	public:
		Chunk(const std::string & name);
		~Chunk() noexcept;
		Chunk(const Chunk & other) noexcept = default;
		Chunk(Chunk &&) noexcept = default;
		Chunk & operator=(Chunk &) = delete;
		Chunk & operator=(Chunk &&) = delete;

		void SetName(const std::string & name) { this->name_ = name; }
		const std::string & GetName() const { return this->name_; }

		// Get a list of transformable elements
		// Returns a list of child entities
		virtual const std::vector<std::unique_ptr<Entity>> & GetChildTransformables() override { return entities_; }

	private:
		// Name of the chunk
		std::string name_;
	};
}
