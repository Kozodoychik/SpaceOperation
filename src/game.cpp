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
#define ENEMY_CHANCE 10000 // 1 к 10000

#define FRAME_TIME 1.0 / 60.0

SpriteRenderer* Game::sprite_renderer;
UIRenderer* Game::ui_renderer;
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

	this->sprite_renderer = new SpriteRenderer(this->renderer);
	this->ui_renderer = new UIRenderer(this->renderer);

	this->state = RUNNING;

	this->bg_scroll = 0;
	this->background = new Spritesheet(this->renderer, "assets/background.png", 480, 1280);

	return 0;

}

void Game::MainLoop() {
	Image* hp_bar_back = new Image(this->renderer, "assets/hp_bar.png", 0, 0, 244, 20);
	ProgressBar* hp_bar = new ProgressBar(this->renderer, 100, 0, 0, 240);

	Label* hp_label = new Label(this->renderer, "HP: 0", 0, 20, 16);
	Label* score_label = new Label(this->renderer, "00000000", 352, 20, 16);

	this->ui_renderer->AddWidget(hp_bar_back);
	this->ui_renderer->AddWidget(hp_bar);
	this->ui_renderer->AddWidget(hp_label);
	this->ui_renderer->AddWidget(score_label);

	this->player = new Player(this->sprite_renderer);
	this->sprite_renderer->AddSprite(this->player);

	logging::InfoColored(MODULE_NAME, "Entering MainLoop");

	SDL_Event e;

	double dt = 0;

	while (this->state != STOPPED) {
		Uint64 frame_start = SDL_GetPerformanceCounter();

		hp_bar->value = this->player->hp;
		hp_label->text = std::format("HP: {}", this->player->hp);
		score_label->text = std::format("{:0>8}", this->player->score);

		SDL_SetRenderDrawColor(this->renderer, 0, 0, 40, 255);
		SDL_RenderClear(this->renderer);

		this->RenderBackground(dt);

		switch (this->state) {
		case RUNNING: {

			//this->GenerateAsteroids();
			this->GenerateEnemies();
			this->sprite_renderer->Render(dt);

			while (SDL_PollEvent(&e)) {
				if (e.type == EVENT_GAMEOVER) {
					Label* game_over_label = new Label(this->renderer, "GAME OVER!", 160, 312, 16);
					this->ui_renderer->AddWidget(game_over_label);

					this->state = GAMEOVER;
				}

				this->sprite_renderer->HandleEvent(e);
				this->HandleDefaultEvent(e);
			}
			break;

		}
		case GAMEOVER: {

			while (SDL_PollEvent(&e)) {
				this->HandleDefaultEvent(e);
			}
			break;

		}
		}

		this->ui_renderer->Render(dt);

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

void Game::RenderBackground(double dt) {
	this->bg_scroll += 20 * dt;

	if (this->bg_scroll >= 1280)
		this->bg_scroll = 0;

	SDL_Rect lower = {
		0, (int)this->bg_scroll,
		480, 1280
	};

	SDL_Rect upper = {
		0, (int)this->bg_scroll - 1280,
		480, 1280
	};

	this->background->CopyTileTo(0, 0, &lower);
	this->background->CopyTileTo(0, 0, &upper);
}

void Game::HandleDefaultEvent(SDL_Event e) {
	switch (e.type) {
	case SDL_QUIT: this->state = STOPPED; break;
	}
}