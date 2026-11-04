#ifndef GAME_HPP
#define GAME_HPP

#include <SDL.h>
#include <vector>
#include "sprite.hpp"
#include "ui.hpp"
#include "player.hpp"

class Game {

private:
	
	typedef enum {

		RUNNING,
		GAMEOVER,
		STOPPED

	} GameState;

	SDL_Window* window;
	SDL_Renderer* renderer;

	Spritesheet* background;

	GameState state;

	double bg_scroll;

	void GenerateAsteroids();
	void GenerateEnemies();
	void RenderBackground(double dt);
	void HandleDefaultEvent(SDL_Event e);

public:

	static SpriteRenderer* sprite_renderer;
	static UIRenderer* ui_renderer;
	static Player* player;

	int Initialize();

	void MainLoop();
};

#endif