#ifndef GAME_HPP
#define GAME_HPP

#include <SDL.h>
#include <vector>
#include "level.hpp"
#include "menu.hpp"
#include "player.hpp"

class Game {

private:
	
	typedef enum {

		RUNNING,
		FREEZE,
		GAMEOVER,
		STOPPED

	} GameState;

	SDL_Window* window;
	SDL_Renderer* renderer;

	Spritesheet* background;

	GameState state;

	double bg_scroll;

	void HandleDefaultEvent(SDL_Event e);

public:

	static LevelManager* level_manager;
	static MenuManager* menu_manager;
	static Player* player;

	int Initialize();

	void MainLoop();
};

#endif