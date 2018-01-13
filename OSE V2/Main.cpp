#include "Main.h"
#include "Transform.h"

int main()
{
	using namespace origami_sheep_engine;

	Transform t { { 0, 832043982, -432098 }, { 0, 0, 0 }, { 1, 1, 1 } };
	//t.rotate(3.14159265359, 0, 0);
	//t.rotateDeg({180, 0, 0});
	DEBUG_LOG(t.get_up().x << ", " << t.get_up().y << ", " << t.get_up().z);
	DEBUG_LOG(t.get_forward().x << ", " << t.get_forward().y << ", " << t.get_forward().z);
	DEBUG_LOG(t.get_right().x << ", " << t.get_right().y << ", " << t.get_right().z);

	std::unique_ptr<Game> game = std::make_unique<Game>();
	game->loadProject("OSE-V2-TestProject");
	game->loadScene("scene1");
	game->startGame();
	getchar();
	return 0;
}
