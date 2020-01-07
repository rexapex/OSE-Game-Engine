#pragma once

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

		virtual void LoadScene(const std::string & scene_name) = 0;
		virtual void UnloadScene(const std::string & scene_name) = 0;
		virtual void UnloadAllLoadedScenes() = 0;

	protected:
		std::unique_ptr<Project> project_;
		std::unique_ptr<ProjectLoader> project_loader_;

		virtual void OnSceneActivated(Scene & scene) = 0;
		virtual void OnSceneDeactivated(Scene & scene) = 0;
		virtual void OnProjectActivated(Project & project) = 0;
		virtual void OnProjectDeactivated(Project & project) = 0;
	};
}
