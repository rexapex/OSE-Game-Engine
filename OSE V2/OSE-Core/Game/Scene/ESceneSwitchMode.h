#pragma once

namespace ose
{
	enum class ESceneSwitchMode
	{
		REMOVE_ALL_ON_SWITCH = 0,			//remove all loaded scenes (except new active scene) and previous active scene
		REMOVE_LOADED_ON_SWITCH = 1,		//remove all loaded scenes (except new active scene)
		REMOVE_ACTIVE_ON_SWITCH = 2,		//remove only the previous active scene
		REMOVE_NONE_ON_SWITCH = 3			//remove no scenes, previous active scene will be added to loaded scenes
	};
}
