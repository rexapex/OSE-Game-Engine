#pragma once
#include "OSE-Core/Entity/EntityList.h"

namespace ose
{
	class Game;

	class Chunk : public EntityList
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

		bool IsEnabled() const { return enabled_; }
		void SetEnabled(bool a);
		void Enable();
		void Disable();

		// Should NEVER be called directly by a script
		void SetGameReference(unowned_ptr<Game> game) { game_ = game; }

	private:
		// Name of the chunk
		std::string name_;

		bool enabled_ { true };	// True iff the chunk is enabled (i.e. it appears in the scene)

		unowned_ptr<Game> game_ { nullptr }; // Pointer to the game object this chunk belongs to
	};
}
