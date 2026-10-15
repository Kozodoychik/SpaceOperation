#include <SDL.h>
#include <vector>
#include "sprite.hpp"


class Game {

private:
	SDL_Window* window;
	SDL_Renderer* renderer;

	SpriteRenderer* sprite_renderer;

public:
	int Initialize();

	void MainLoop();
	void GenerateAsteroids();

};