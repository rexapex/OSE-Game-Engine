#pragma once
#include "SceneManager.h"
#include "ESceneSwitchMode.h"

namespace ose::game
{
	class Scene;
	
	class SceneSwitchManager : public SceneManager
	{
	public:
		SceneSwitchManager();
		virtual ~SceneSwitchManager() noexcept;
		SceneSwitchManager(SceneSwitchManager &) = delete;
		SceneSwitchManager & operator=(SceneSwitchManager &) = delete;

		virtual void LoadScene(const std::string & scene_name);
		virtual void UnloadScene(const std::string & scene_name);
		virtual void UnloadAllLoadedScenes();

		// Switches game to the scene specified iff it is loaded
		void SetActiveScene(const std::string & scene_name);

		// Set the way the game removes scenes on a scene switch
		void SetSceneSwitchMode(const ESceneSwitchMode & mode) {scene_switch_mode_ = mode;}

	private:
		// Specifies which scenes should be unloaded on scene switch
		ESceneSwitchMode scene_switch_mode_;

		// The current scene being played (updated, rendered, etc...)
		std::unique_ptr<Scene> active_scene_;

		// Contains every scene which has been loaded but NOT the active scene
		std::map<std::string, std::unique_ptr<Scene>> loaded_scenes_;
	};
}
