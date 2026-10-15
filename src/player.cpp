#include <SDL.h>
#include "player.hpp"
#include "spritesheet.hpp"
#include "logging.hpp"
#include "bullet.hpp"

#define PLAYER_SPEED 250.0
#define PLAYER_Y_PAD 40

#define PLAYER_SPAWN_X 205
#define PLAYER_SPAWN_Y 570 - PLAYER_Y_PAD

Player::Player(SpriteRenderer* renderer) : Sprite(renderer, "assets/spaceship.png", PLAYER_SPAWN_X, PLAYER_SPAWN_Y, 70, 70) {
	this->key_map.flags = 0;
	this->bullet_timer = 0;
}

Player::~Player() {

}

void Player::Render(double dt) {
	if (this->is_moving) {
		this->pos.x += this->key_map.d * (PLAYER_SPEED * dt);
		this->pos.x -= this->key_map.a * (PLAYER_SPEED * dt);
		this->pos.y += this->key_map.s * (PLAYER_SPEED * dt);
		this->pos.y -= this->key_map.w * (PLAYER_SPEED * dt);
	}

	if (this->key_map.space) {
		if (this->bullet_timer >= 0.08) {
			Bullet* bullet = new Bullet(this->sprite_renderer, this->pos.x + 33, this->pos.y - 10);
			this->sprite_renderer->AddSprite(bullet);

			this->bullet_timer = 0;
		}
		this->bullet_timer += dt;
	}

	Sprite::Render(dt);
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