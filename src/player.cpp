#include <SDL.h>
#include "player.hpp"
#include "spritesheet.hpp"
#include "logging.hpp"
#include "bullet.hpp"
#include "asteroid.hpp"
#include "events.hpp"

#define PLAYER_SPEED 250.0
#define PLAYER_Y_PAD 40

#define PLAYER_SPAWN_X 205
#define PLAYER_SPAWN_Y 570 - PLAYER_Y_PAD

#define PLAYER_HP 100

#define BULLET_COOLDOWN 0.08

Player::Player(SpriteRenderer* renderer) : Sprite(renderer, "assets/spaceship.png", PLAYER_SPAWN_X, PLAYER_SPAWN_Y, 70, 70) {
	this->key_map.flags = 0;
	this->bullet_timer = 0;
	this->hit_timer = 0;
	this->hp = PLAYER_HP;

	this->hitbox = {
		19, 8,
		32, 56
	};
}

void Player::Render(double dt) {
	if (this->is_moving) {
		this->pos.x += this->key_map.d * (PLAYER_SPEED * dt);
		this->pos.x -= this->key_map.a * (PLAYER_SPEED * dt);
		this->pos.y += this->key_map.s * (PLAYER_SPEED * dt);
		this->pos.y -= this->key_map.w * (PLAYER_SPEED * dt);
	}

	if (this->key_map.space) {
		if (this->bullet_timer >= BULLET_COOLDOWN) {
			Bullet* bullet = new Bullet(this->sprite_renderer, (int)this->pos.x + 33, (int)this->pos.y - 10, {0, 1});
			this->sprite_renderer->AddSprite(bullet);

			this->bullet_timer = 0;
		}
		this->bullet_timer += dt;
	}

	if (this->hit_timer > 0) {
		if ((int)this->hit_timer % 2 == 0)
			Sprite::Render(dt);

		this->hit_timer -= 4 * dt;
	}
	else {
		Sprite::Render(dt);
	}
}

void Player::OnEvent(SDL_Event e) {
	Sprite::OnEvent(e);

	switch (e.type) {
	case SDL_KEYUP:
	case SDL_KEYDOWN: {
		bool is_keydown = e.type == SDL_KEYDOWN;

		this->key_map.w = 
			((e.key.keysym.scancode == SDL_SCANCODE_W) || this->key_map.w) &&
			((e.key.keysym.scancode != SDL_SCANCODE_W) || is_keydown);
		this->key_map.a = 
			((e.key.keysym.scancode == SDL_SCANCODE_A) || this->key_map.a) &&
			((e.key.keysym.scancode != SDL_SCANCODE_A) || is_keydown);
		this->key_map.s = 
			((e.key.keysym.scancode == SDL_SCANCODE_S) || this->key_map.s) && 
			((e.key.keysym.scancode != SDL_SCANCODE_S) || is_keydown);
		this->key_map.d =
			((e.key.keysym.scancode == SDL_SCANCODE_D) || this->key_map.d) && 
			((e.key.keysym.scancode != SDL_SCANCODE_D) || is_keydown);

		this->key_map.space = 
			((e.key.keysym.scancode == SDL_SCANCODE_SPACE) || this->key_map.space) && 
			((e.key.keysym.scancode != SDL_SCANCODE_SPACE) || is_keydown);

		this->is_moving = this->key_map.flags != 0;

		break;
	}
	}
}

void Player::OnCollision(Sprite* collided) {
	if (dynamic_cast<const Asteroid*>(collided) != nullptr && this->hit_timer <= 0) {
		this->sprite_renderer->DestroySprite(collided);
		this->hp -= 10;
		this->hit_timer = 20;

		if (this->hp == 0) {
			SDL_Event e{EVENT_GAME_OVER};
			SDL_PushEvent(&e);
			this->sprite_renderer->DestroySprite(this);
		}
	}
}