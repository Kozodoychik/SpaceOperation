#include <SDL.h>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <iostream>
#include "asteroid.hpp"
#include "bullet.hpp"
#include "logging.hpp"
#include "spritesheet.hpp"

#define MAX_ACCELERATION 10
#define MAX_ROTATION_SPEED 60

#define ANIMATION_FPS 32
#define ANIMATION_FRAME_COUNT 17

Asteroid::Asteroid(SpriteRenderer* renderer, int x, int y) : Sprite(renderer, "assets/asteroid.png", x, y, 46, 43) {
	srand((unsigned int)SDL_GetPerformanceCounter());

	this->acceleration = 10;
	this->acceleration = rand() % MAX_ACCELERATION;

	this->size_type = rand() % 4;

	this->size.w *= this->size_type;
	this->size.h *= this->size_type;

	this->rotation_angle = rand() % 360;
	this->rotation_speed = rand() % MAX_ROTATION_SPEED * pow(-1, rand() % 2);
	this->hp = 5 + (this->size_type * 5);
	this->is_destroyed = false;

	this->explosion = new Spritesheet(renderer->renderer, "assets/explosion.png", 142, 172);
	this->animation_counter = 0;

	this->hitbox = {
		4, 4,
		38 * this->size_type, 33 * this->size_type
	};
}

void Asteroid::Render(double dt) {
	SDL_Rect rect = {
		(int)this->pos.x, (int)this->pos.y,
		(int)this->size.w, (int)this->size.h
	};

	if (this->is_destroyed) {
		int frame = (int)this->animation_counter % ANIMATION_FRAME_COUNT;

		this->explosion->CopyTileTo(frame, 0, &rect);
		
		if (frame >= ANIMATION_FRAME_COUNT-1)
			this->sprite_renderer->DestroySprite(this);

		this->animation_counter += ANIMATION_FPS * dt;
		return;
	}
	
	Sprite::Render(dt);

	this->rotation_angle += this->rotation_speed * dt;

	if (this->rotation_angle > 360) this->rotation_angle = 0;
	if (this->rotation_angle < 0) this->rotation_angle = 360;

	this->pos.y += 10 * dt * acceleration;
	this->acceleration += dt;

	if (this->pos.y >= 640)
		this->sprite_renderer->DestroySprite(this);
}

void Asteroid::OnCollision(Sprite* collided) {
	if (dynamic_cast<const Bullet*>(collided) != nullptr && !this->is_destroyed) {
		this->sprite_renderer->DestroySprite(collided);
		this->hp--;

		if (this->hp == 0) {
			this->is_destroyed = true;
		}
	}
}