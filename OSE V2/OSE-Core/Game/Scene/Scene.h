#pragma once

#include "OSE-Core/Entity/EntityList.h"

namespace ose
{
	class Scene : public EntityList
	{
	public:
		Scene(const std::string & name);
		virtual ~Scene() noexcept;
		Scene(const Scene & other) noexcept;
		Scene(Scene &&) noexcept = default;
		Scene & operator=(Scene &) = delete;
		Scene & operator=(Scene &&) = delete;
		
		// Get the name of the scene
		const std::string & GetName() const { return this->name_; }

		// DEBUG METHODS
		// TODO - REMOVE WHEN READY
		void Print();
		void PrintEntity(const Entity & e, int32_t level);

	private:
		std::string name_;
	};
}

