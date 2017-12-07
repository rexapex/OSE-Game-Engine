#pragma once

#include "Project.h"
#include "ProjectLoader.h"
#include "ProjectLoaderXML.h"
#include "Scene.h"
#include "ESceneSwitchMode.h"

namespace origami_sheep_engine
{
	//Represents a runtime object of a game
	//Provides a simple way of interacting with the game
	class Game
	{
	public:
		Game();
		~Game() noexcept;
		Game(Game &) = delete;
		Game & operator=(Game &) = delete;
		Game(Game && other) noexcept;
		Game & operator=(Game && other) noexcept;
		
		void loadProject(const std::string & proj_name);		//loads the project specified (does not load any scenes)
		void loadScene(const std::string & scene_name);			//loads the scene into memory
		void unloadScene(const std::string & scene_name);		//frees the scene from memory
		void unloadAllLoadedScenes();							//frees all loaded scenes (not active scene) from memory
		void setActiveScene(const std::string & scene_name);	//switches game to the scene specified iff it is loaded

		//set the way the game removes scenes on a scene switch
		void setSceneSwitchMode(const ESceneSwitchMode & mode) {scene_switch_mode_ = mode;}

		//start execution of the game
		void startGame();

	private:
		//specifies which scenes should be unloaded on scene switch
		ESceneSwitchMode scene_switch_mode_;

		std::unique_ptr<Project> project_;
		std::unique_ptr<ProjectLoader> project_loader_;

		//the current scene being played (updated, rendered, etc...)
		std::unique_ptr<Scene> active_scene_;

		//contains every scene which has been loaded but NOT the active scene
		std::map<std::string, std::unique_ptr<Scene>> loaded_scenes_;

		//true iff the game is currently running (paused is a subset of running)
		bool running_;

		//called from startGame, runs a loop while running_ is true
		void runGame();
	};
}
