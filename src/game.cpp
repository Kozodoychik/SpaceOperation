#include <SDL.h>
#include <iostream>
#include <format>
#include "game.hpp"
#include "logging.hpp"
#include "spritesheet.hpp"
#include "player.hpp"


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

	this->renderer = SDL_CreateRenderer(this->window, 0, SDL_RENDERER_ACCELERATED);

	if (this->renderer == NULL) {
		logging::Fatal(MODULE_NAME, std::format("Cannot create renderer: {}", SDL_GetError()).c_str());
		return -1;
	}

	logging::Info(MODULE_NAME, "Created window");

	Player* p = new Player(this->renderer);

	this->sprites.push_back((Sprite*)p);

	return 0;

}

void Game::MainLoop() {

	logging::InfoColored(MODULE_NAME, "Entering MainLoop");

	SDL_Event e;
	double frame_time = 1.0 / 60.0;

	bool is_running = true;
	double dt = 0;

	while (is_running) {
		Uint64 frame_start = SDL_GetPerformanceCounter();

		// Отображаем спрайты

		SDL_RenderClear(this->renderer);

		for (auto sprite : this->sprites) {
			sprite->Render(dt);
		}

		SDL_RenderPresent(this->renderer);
		SDL_UpdateWindowSurface(this->window);

		// Обрабатываем события

		while (SDL_PollEvent(&e)) {
			switch (e.type) {
			case SDL_QUIT: {
				is_running = false;
				break;
			}
			default: {
				for (auto sprite : this->sprites) {
					sprite->OnEvent(e);
				}
			}
			}
		}

		// Подсчитываем deltaTime и ограничиваем FPS до 60

		Uint64 frame_end = SDL_GetPerformanceCounter();

		dt = ((double)(frame_end - frame_start) / (double)SDL_GetPerformanceFrequency());

		if (dt < frame_time) {
			SDL_Delay((Uint32)floor(frame_time - dt) * 1000.0);
		}
	}

	logging::InfoColored(MODULE_NAME, "Exited MainLoop");
	logging::Info(MODULE_NAME, "Destroying window and renderer...");
	SDL_DestroyRenderer(this->renderer);
	SDL_DestroyWindow(this->window);

	logging::Info(MODULE_NAME, "Goodbye!");
	SDL_Quit();
}