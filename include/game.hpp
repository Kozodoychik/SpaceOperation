#include <SDL.h>
#include <vector>
#include "sprite.hpp"
#include "ui.hpp"


class Game {

private:
	SDL_Window* window;
	SDL_Renderer* renderer;

	SpriteRenderer* sprite_renderer;
	UIRenderer* ui_renderer;

public:

	int Initialize();

	void MainLoop();
	void GenerateAsteroids();

};