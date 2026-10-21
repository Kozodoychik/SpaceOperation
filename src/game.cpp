#include <SDL.h>
#include <iostream>
#include <format>
#include <cstdlib>
#include "game.hpp"
#include "logging.hpp"
#include "spritesheet.hpp"
#include "player.hpp"
#include "asteroid.hpp"
#include "enemy.hpp"
#include "ui.hpp"
#include "events.hpp"

#define ASTEROID_CHANCE 5000 // 1 к 5000
#define ENEMY_CHANCE 100000 // 1 к 100000

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

	this->renderer = SDL_CreateRenderer(this->window, 0, SDL_RENDERER_ACCELERATED);

	if (this->renderer == NULL) {
		logging::Fatal(MODULE_NAME, std::format("Cannot create renderer: {}", SDL_GetError()).c_str());
		return -1;
	}

	logging::Info(MODULE_NAME, "Created window");

	this->sprite_renderer = new SpriteRenderer(this->renderer);
	this->ui_renderer = new UIRenderer(this->renderer);

	this->is_running = true;
	this->is_gameover = false;

	return 0;

}

void Game::MainLoop() {
	Label* hp_label = new Label(this->renderer, "HP: 0", 0, 0, 16);
	this->ui_renderer->AddWidget(hp_label);

	Player* player = new Player(this->sprite_renderer);
	this->sprite_renderer->AddSprite(player);

	logging::InfoColored(MODULE_NAME, "Entering MainLoop");

	SDL_Event e;
	double frame_time = 1.0 / 60.0;

	double dt = 0;

	while (this->is_running) {
		Uint64 frame_start = SDL_GetPerformanceCounter();

		hp_label->text = std::format("HP: {}", player->hp);

		SDL_SetRenderDrawColor(this->renderer, 0, 0, 40, 255);
		SDL_RenderClear(this->renderer);

		// Отображаем спрайты

		if (!this->is_gameover) {
			this->GenerateAsteroids();
			this->GenerateEnemies();
			this->sprite_renderer->Render(dt);
		}

		// Отображаем элементы интерфейса

		this->ui_renderer->Render(dt);

		// Выводим на экран

		SDL_RenderPresent(this->renderer);
		SDL_UpdateWindowSurface(this->window);

		// Обрабатываем события

		while (SDL_PollEvent(&e)) {
			// Событие проигрыша
			if (e.type == EVENT_GAME_OVER) {
				Label* game_over_label = new Label(this->renderer, "GAME OVER!", 160, 312, 16);
				this->ui_renderer->AddWidget(game_over_label);

				this->is_gameover = true;
			}

			// Обрабатываем остальные события
			switch (e.type) {
			case SDL_QUIT: {
				this->is_running = false;
				break;
			}
			default: {
				if (!this->is_gameover)
					this->sprite_renderer->HandleEvent(e);
			}
			}
		}

		// Подсчитываем deltaTime и ограничиваем FPS до 60

		Uint64 frame_end = SDL_GetPerformanceCounter();

		dt = ((double)(frame_end - frame_start) / (double)SDL_GetPerformanceFrequency());

		if (dt < frame_time) {
			SDL_Delay((Uint32)(floor(frame_time - dt) * 1000.0));
		}
	}

	logging::InfoColored(MODULE_NAME, "Exited MainLoop");
	logging::Info(MODULE_NAME, "Destroying window and renderer...");
	SDL_DestroyRenderer(this->renderer);
	SDL_DestroyWindow(this->window);

	delete this->sprite_renderer;
	delete this->ui_renderer;

	logging::Info(MODULE_NAME, "Goodbye!");
	SDL_Quit();
}

void Game::GenerateAsteroids() {
	srand(SDL_GetTicks());
	
	int x = rand() % 480;

	if (rand() % ASTEROID_CHANCE == 0) {
		Asteroid* asteroid = new Asteroid(this->sprite_renderer, x - 70, -300);
		this->sprite_renderer->AddSprite(asteroid);
	}
}

void Game::GenerateEnemies() {
	srand(SDL_GetTicks());

	int x = rand() % 480;

	if (rand() % ENEMY_CHANCE == 0) {
		Enemy* enemy = new Enemy(this->sprite_renderer, x - 70, -50);
		this->sprite_renderer->AddSprite(enemy);
	}
}