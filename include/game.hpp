#include <SDL.h>


class Game {

private:
	SDL_Window* window;
	SDL_Renderer* renderer;

public:
	int Initialize();

	void MainLoop();

};