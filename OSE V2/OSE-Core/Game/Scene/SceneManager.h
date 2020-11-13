#pragma once
#include "ESceneSwitchMode.h"

namespace ose
{
	class Scene;
	class Project;
	class ProjectLoader;
	class Chunk;

	class SceneManager
	{
	public:
		SceneManager();
		virtual ~SceneManager() noexcept;
		SceneManager(SceneManager &) = delete;
		SceneManager & operator=(SceneManager &) = delete;

		// Loads the project specified (does not load any scenes)
		void LoadProject(std::string const & project_path);

		void LoadScene(std::string const & scene_name);
		void UnloadScene(std::string const & scene_name);
		void UnloadAllLoadedScenes();

		// Switches game to the scene specified iff it is loaded
		void SetActiveScene(std::string const & scene_name);

		// Set the way the game removes scenes on a scene switch
		void SetSceneSwitchMode(ESceneSwitchMode const & mode) {scene_switch_mode_ = mode;}

		// Called by a scene (chunk manager) upon activating/deactivating a chunk
		virtual void OnChunkActivated(Chunk & chunk) = 0;
		virtual void OnChunkDeactivated(Chunk & chunk) = 0;

	protected:
		uptr<Project> project_;
		uptr<ProjectLoader> project_loader_;

		virtual void OnSceneActivated(Scene & scene) = 0;
		virtual void OnSceneDeactivated(Scene & scene) = 0;
		virtual void OnProjectActivated(Project & project) = 0;
		virtual void OnProjectDeactivated(Project & project) = 0;

		// The current scene being played (updated, rendered, etc...)
		uptr<Scene> active_scene_;

	private:
		// Specifies which scenes should be unloaded on scene switch
		ESceneSwitchMode scene_switch_mode_;

		// Contains every scene which has been loaded but NOT the active scene
		std::map<std::string, uptr<Scene>> loaded_scenes_;
	};
}
