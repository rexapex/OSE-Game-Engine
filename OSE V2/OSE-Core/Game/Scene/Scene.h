#pragma once

#include "OSE-Core/Entity/EntityList.h"
#include "OSE-Core/Scripting/ControlSettings.h"
#include "OSE-Core/Game/Scene/Chunk/ChunkManager.h"

namespace ose
{
	class ProjectLoader;
	class SceneManager;

	class Scene : public EntityList, public ChunkManager
	{
	public:
		Scene(const std::string & name, const ControlSettings & control_settings);
		virtual ~Scene() noexcept;
		Scene(const Scene & other) noexcept;
		Scene(Scene &&) noexcept = default;
		Scene & operator=(Scene &) = delete;
		Scene & operator=(Scene &&) = delete;
		
		// Get the name of the scene
		const std::string & GetName() const { return this->name_; }

		// Get the control script settings of the scene
		const ControlSettings & GetControlSettings() const { return control_settings_; }

		// Set the scene manager managing this scene
		void SetSceneManager(SceneManager * s) { scene_manager_ = s; }

		// DEBUG METHODS
		// TODO - REMOVE WHEN READY
		void Print();
		void PrintEntity(const Entity & e, int32_t level);

	protected:
		virtual void OnChunkActivated(Chunk & chunk) override;
		virtual void OnChunkDeactivated(Chunk & chunk) override;

	private:
		// Name of the scene
		// Must be unique among scenes
		std::string name_;

		// Array of control scripts required by the scene in the order they should be run
		ControlSettings control_settings_;

		// The scene manager which is managing this scene
		SceneManager * scene_manager_ { nullptr };
	};
}

