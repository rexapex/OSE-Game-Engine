#pragma once
#include "ESceneSwitchMode.h"

namespace ose
{
	class Scene;
	class Project;
	class ProjectLoader;

	class SceneManager
	{
	public:
		SceneManager();
		virtual ~SceneManager() noexcept;
		SceneManager(SceneManager &) = delete;
		SceneManager & operator=(SceneManager &) = delete;

		// Loads the project specified (does not load any scenes)
		void LoadProject(const std::string & proj_name);

		void LoadScene(const std::string & scene_name);
		void UnloadScene(const std::string & scene_name);
		void UnloadAllLoadedScenes();

		// Switches game to the scene specified iff it is loaded
		void SetActiveScene(const std::string & scene_name);

		// Set the way the game removes scenes on a scene switch
		void SetSceneSwitchMode(const ESceneSwitchMode & mode) {scene_switch_mode_ = mode;}

	protected:
		std::unique_ptr<Project> project_;
		std::unique_ptr<ProjectLoader> project_loader_;

		virtual void OnSceneActivated(Scene & scene) = 0;
		virtual void OnSceneDeactivated(Scene & scene) = 0;
		virtual void OnProjectActivated(Project & project) = 0;
		virtual void OnProjectDeactivated(Project & project) = 0;

	private:
		// Specifies which scenes should be unloaded on scene switch
		ESceneSwitchMode scene_switch_mode_;

		// The current scene being played (updated, rendered, etc...)
		std::unique_ptr<Scene> active_scene_;

		// Contains every scene which has been loaded but NOT the active scene
		std::map<std::string, std::unique_ptr<Scene>> loaded_scenes_;
	};
}
