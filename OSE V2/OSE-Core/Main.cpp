#include "stdafx.h"
#include "Main.h"
#include "Math/Transform.h"

int main()
{
	using namespace ose::math;
	using namespace ose::game;

	Transform t { { 0, 832043982, -432098 }, { 0, 0, 0 }, { 1, 1, 1 } };
	//t.rotate(3.14159265359, 0, 0);
	//t.rotateDeg({180, 0, 0});
	DEBUG_LOG(t.get_up().x << ", " << t.get_up().y << ", " << t.get_up().z);
	DEBUG_LOG(t.get_forward().x << ", " << t.get_forward().y << ", " << t.get_forward().z);
	DEBUG_LOG(t.get_right().x << ", " << t.get_right().y << ", " << t.get_right().z);

	std::unique_ptr<Game> game = std::make_unique<Game>();
	
	// TODO - might need to destroy resources before returning error

	try {
		game->loadProject("OSE-V2-TestProject");
	} catch(const std::invalid_argument & e) {
		ERROR_LOG(e.what());
		return 1;
	} catch(const std::exception & e) {
		ERROR_LOG(e.what());
		return 1;
	}

	try {
		game->loadScene("scene1");
	} catch(const std::invalid_argument & e) {
		ERROR_LOG(e.what());
		return 1;
	} catch(const std::exception & e) {
		ERROR_LOG(e.what());
		return 1;
	}

	game->startGame();

	return 0;
}
