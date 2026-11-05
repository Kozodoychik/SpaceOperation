#include <SDL.h>
#include <iostream>
#include <format>
#include <cstdlib>
#include "game.hpp"
#include "logging.hpp"
#include "spritesheet.hpp"
#include "menu.hpp"
#include "events.hpp"

#define FRAME_TIME 1.0 / 60.0

LevelManager* Game::level_manager;
MenuManager* Game::menu_manager;
Player* Game::player;

int Game::Initialize() {

	logging::Info(MODULE_NAME, "Initializing SDL...");

	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		logging::Fatal(MODULE_NAME, std::format("Cannot initialize SDL: {}", SDL_GetError()).c_str());
		return -1;
	}

	this->window = SDL_CreateWindow(
		"Space Operation",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		480, 640,
		0
	);

	if (this->window == NULL) {
		logging::Fatal(MODULE_NAME, std::format("Cannot create window: {}", SDL_GetError()).c_str());
		return -1;
	}

	this->renderer = SDL_CreateRenderer(this->window, 0, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);

	if (this->renderer == NULL) {
		logging::Fatal(MODULE_NAME, std::format("Cannot create renderer: {}", SDL_GetError()).c_str());
		return -1;
	}

	logging::Info(MODULE_NAME, "Created window");

	this->level_manager = new LevelManager(renderer);
	this->menu_manager = new MenuManager(this->renderer);

	this->state = FREEZE;

	return 0;

}

void Game::MainLoop() {

	logging::InfoColored(MODULE_NAME, "Entering MainLoop");

	SDL_Event e;

	double dt = 0;

	while (this->state != STOPPED) {
		Uint64 frame_start = SDL_GetPerformanceCounter();

		SDL_SetRenderDrawColor(this->renderer, 0, 0, 40, 255);
		SDL_RenderClear(this->renderer);

		this->level_manager->RenderBackground(dt);

		switch (this->state) {
		case RUNNING: {

			this->level_manager->GenerateObstacles();

			while (SDL_PollEvent(&e)) {
				if (e.type == EVENT_GAMEOVER) {
					this->state = GAMEOVER;
				}

				this->level_manager->HandleEvent(e);
				this->HandleDefaultEvent(e);
			}

			this->level_manager->Render(dt);

			break;

		}
		case FREEZE:
		case GAMEOVER: {

			while (SDL_PollEvent(&e)) {
				this->HandleDefaultEvent(e);
			}
			break;

		}
		}

		this->menu_manager->Render(dt);

		SDL_RenderPresent(this->renderer);
		SDL_UpdateWindowSurface(this->window);

		// Подсчитываем deltaTime и ограничиваем FPS до 60

		Uint64 frame_end = SDL_GetPerformanceCounter();

		dt = ((double)(frame_end - frame_start) / (double)SDL_GetPerformanceFrequency());

		if (dt < FRAME_TIME) {
			SDL_Delay((Uint32)(floor(FRAME_TIME - dt) * 1000.0));
		}
	}

	logging::InfoColored(MODULE_NAME, "Exited MainLoop");
	logging::Info(MODULE_NAME, "Destroying window and renderer...");
	SDL_DestroyRenderer(this->renderer);
	SDL_DestroyWindow(this->window);

	delete this->level_manager;
	delete this->menu_manager;

	logging::Info(MODULE_NAME, "Goodbye!");
	SDL_Quit();
}

void Game::HandleDefaultEvent(SDL_Event e) {
	this->menu_manager->HandleEvent(e);

	switch (e.type) {
	case SDL_QUIT: this->state = STOPPED; break;
	}

	if (e.type == EVENT_GAME_FREEZE)
		this->state = FREEZE;
	else if (e.type == EVENT_GAME_UNFREEZE)
		this->state = RUNNING;
	else if (e.type == EVENT_GAME_RESET) {
		this->level_manager->Reset();
		this->state = FREEZE;
	}
	else if (e.type == EVENT_GAME_RESTART) {
		this->level_manager->Reset();
		this->state = RUNNING;
	}
}