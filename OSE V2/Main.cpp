#include "Main.h"

int main()
{
	using namespace origami_sheep_engine;
	std::unique_ptr<Game> game = std::make_unique<Game>();
	game->loadProject("OSE-V2-TestProject");
	game->loadScene("scene1");
	game->startGame();
	getchar();
	return 0;
}
