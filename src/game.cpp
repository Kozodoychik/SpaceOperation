#include "game.hpp"
#include "logging.hpp"
#include <SDL.h>
#include <iostream>
#include <format>

int Game::Initialize() {

	logging::Info(MODULE_NAME, "Initializing SDL...");

	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		logging::Fatal(MODULE_NAME, std::format("Cannot initialize SDL: {}", SDL_GetError()).c_str());
		return -1;
	}

	this->window = SDL_CreateWindow(
		"Simple Scroll-Shooter",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		480, 640,
		0
	);

	if (this->window == NULL) {
		logging::Fatal(MODULE_NAME, std::format("Cannot create window: {}", SDL_GetError()).c_str());
		return -1;
	}

	this->renderer = SDL_CreateRenderer(this->window, 0, -1);

	if (this->renderer == NULL) {
		logging::Fatal(MODULE_NAME, std::format("Cannot create renderer: {}", SDL_GetError()).c_str());
		return -1;
	}

	logging::Info(MODULE_NAME, "Created window");

	return 0;

}

void Game::MainLoop() {

	logging::InfoColored(MODULE_NAME, "Entering MainLoop");

	SDL_Event e;
	bool is_running = true;

	while (is_running) {
		while (SDL_PollEvent(&e)) {
			switch (e.type) {
			case SDL_QUIT: {
				is_running = false;
			}
			}
		}
	}
	logging::InfoColored(MODULE_NAME, "Exited MainLoop");
	logging::Info(MODULE_NAME, "Destroying window and renderer...");
	SDL_DestroyRenderer(this->renderer);
	SDL_DestroyWindow(this->window);

	logging::Info(MODULE_NAME, "Goodbye!");
	SDL_Quit();
}