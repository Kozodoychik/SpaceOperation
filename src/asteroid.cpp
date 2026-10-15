#include <SDL.h>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <iostream>
#include "asteroid.hpp"
#include "bullet.hpp"
#include "logging.hpp"

Asteroid::Asteroid(SpriteRenderer* renderer, int x, int y) : Sprite(renderer, "assets/asteroid.png", x, y, 46, 43) {
	this->acceleration = 10;

	srand(SDL_GetPerformanceCounter());

	this->size_type = rand() % 4;

	this->size.w *= this->size_type;
	this->size.h *= this->size_type;

	this->angle = rand() % 360;
	this->rotation_speed = rand() % 60 * pow(-1, rand() % 2);
	this->hp = 5 + (this->size_type * 5);
	this->acceleration = rand() % 10;
}

void Asteroid::Render(double dt) {
	SDL_Rect rect = {
		(int)this->pos.x, (int)this->pos.y,
		(int)this->size.w, (int)this->size.h
	};

	SDL_Point center = { (int)this->size.w / 2, (int)this->size.h / 2 };

	SDL_RenderCopyEx(this->sprite_renderer->renderer, this->texture, NULL, &rect, this->angle, &center, SDL_FLIP_NONE);

	this->angle += this->rotation_speed * dt;

	if (this->angle > 360) this->angle = 0;
	if (this->angle < 0) this->angle = 360;

	this->pos.y += 10 * dt * acceleration;
	this->acceleration += dt;

	if (this->pos.y >= 640)
		this->sprite_renderer->DestroySprite(this);
}

void Asteroid::OnCollision(Sprite* collided) {
	if (dynamic_cast<const Bullet*>(collided) != nullptr) {
		this->sprite_renderer->DestroySprite(collided);
		this->hp--;

		if (this->hp == 0) {
			this->sprite_renderer->DestroySprite(this);
		}
	}
}