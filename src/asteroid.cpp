#include <SDL.h>
#include <cmath>
#include "asteroid.hpp"
#include "bullet.hpp"
#include "spritesheet.hpp"
#include "game.hpp"
#include "events.hpp"

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
	this->state = ALIVE;

	this->explosion = new Spritesheet(renderer->renderer, "assets/explosion.png", 142, 172);
	this->animation_counter = 0;

	this->hitbox = {
		4, 4,
		38 * this->size_type, 33 * this->size_type
	};
}

Asteroid::~Asteroid() {
	delete this->explosion;
}

void Asteroid::Render(double dt) {

	switch (this->state) {
	case ALIVE: {

		Sprite::Render(dt);

		this->rotation_angle += this->rotation_speed * dt;

		if (this->rotation_angle > 360) this->rotation_angle = 0;
		if (this->rotation_angle < 0) this->rotation_angle = 360;

		this->pos.y += 10 * dt * acceleration;
		this->acceleration += dt;

		if (this->pos.y >= 640)
			this->sprite_renderer->DestroySprite(this);

		break;

	}
	case DESTROYED: {

		SDL_Rect rect = {
			(int)this->pos.x, (int)this->pos.y,
			(int)this->size.w, (int)this->size.h
		};

		int frame = (int)this->animation_counter % ANIMATION_FRAME_COUNT;

		this->explosion->CopyTileTo(frame, 0, &rect);

		if (frame >= ANIMATION_FRAME_COUNT - 1)
			this->sprite_renderer->DestroySprite(this);

		this->animation_counter += ANIMATION_FPS * dt;

		break;
	}
	}
}

void Asteroid::OnEvent(SDL_Event e) {
	if (this->state == DESTROYED) return;

	if (e.type == EVENT_COLLISION) {
		Sprite* collided = (Sprite*)e.user.data2;

		if (dynamic_cast<const Bullet*>(collided) != nullptr) {
			this->sprite_renderer->DestroySprite(collided);
			this->hp--;

			if (this->hp == 0) {
				this->state = DESTROYED;

				SDL_Event e{ EVENT_SCORE_ADD };
				e.user.code = 50 + (this->size_type * 25);
				SDL_PushEvent(&e);
			}
		}
		else if (dynamic_cast<const Player*>(collided) != nullptr) {
			if (dynamic_cast<Player*>(collided)->hit_timer <= 0)
				this->state = DESTROYED;

			SDL_Event e{ EVENT_PLAYER_DAMAGE };
			e.user.code = 10;
			SDL_PushEvent(&e);
		}
	}
}