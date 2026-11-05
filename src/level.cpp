#include <SDL.h>
#include "level.hpp"
#include "sprite.hpp"
#include "player.hpp"
#include "asteroid.hpp"
#include "enemy.hpp"

#define ASTEROID_CHANCE 5000 // 1 к 5000
#define ENEMY_CHANCE 50000 // 1 к 50000

#define DIFFICULTY_CHANGE_AT 1000 // Увеличивать сложность каждые 100 очков

LevelManager::LevelManager(SDL_Renderer* renderer) {

	this->difficulty = 0;
	this->renderer = renderer;
	this->sprite_renderer = new SpriteRenderer(this->renderer);
	this->player = new Player(this->sprite_renderer);

	this->sprite_renderer->AddSprite(this->player);

	this->bg_scroll = 0;
	this->background = new Spritesheet(this->renderer, "assets/background.png", 480, 1280);

}

LevelManager::~LevelManager() {
	delete this->sprite_renderer;
}

void LevelManager::GenerateObstacles() {

	int asteroid_chance = ASTEROID_CHANCE - (this->difficulty * 200);
	int enemy_chance = ENEMY_CHANCE - (this->difficulty * 200);

	srand(static_cast<unsigned int>(SDL_GetPerformanceCounter()));

	int x = rand() % 480;

	if (rand() % asteroid_chance == 0) {
		Asteroid* asteroid = new Asteroid(this->sprite_renderer, x - 70, -300);
		this->sprite_renderer->AddSprite(asteroid);
	}

	if (this->difficulty > 2) {
		srand(static_cast<unsigned int>(SDL_GetPerformanceCounter()));

		x = rand() % 480;

		if (rand() % enemy_chance == 0) {
			Enemy* enemy = new Enemy(this->sprite_renderer, x - 70, -50);
			this->sprite_renderer->AddSprite(enemy);
		}
	}

	if (this->difficulty < 20)
		this->difficulty = this->player->score / DIFFICULTY_CHANGE_AT;

}

void LevelManager::Reset() {

	delete this->sprite_renderer;

	this->sprite_renderer = new SpriteRenderer(this->renderer);
	this->player = new Player(this->sprite_renderer);

	this->sprite_renderer->AddSprite(this->player);

	this->difficulty = 0;

}

void LevelManager::Render(double dt) {
	this->sprite_renderer->Render(dt);
}

void LevelManager::HandleEvent(SDL_Event e) {
	this->sprite_renderer->HandleEvent(e);
}

void LevelManager::RenderBackground(double dt) {
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