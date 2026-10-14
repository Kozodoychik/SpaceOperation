#include <SDL.h>
#include <vector>
#include "sprite.hpp"


class Game {

private:
	SDL_Window* window;
	SDL_Renderer* renderer;

	std::vector<Sprite*> sprites;

public:
	int Initialize();

	void MainLoop();

};