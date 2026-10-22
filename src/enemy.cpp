#include "enemy.hpp"
#include "sprite.hpp"
#include "bullet.hpp"
#include "game.hpp"

#define ENEMY_SPEED 150

#define BULLET_COOLDOWN 0.5

#define ANIMATION_FPS 32
#define ANIMATION_FRAME_COUNT 17

Enemy::Enemy(SpriteRenderer* renderer, int x, int y) : Sprite(renderer, "assets/enemy.png", x, y, 82, 42) {
	this->rotation_angle = 180;
	this->bullet_timer = 0;
	this->hp = 4;
	this->x_speed = 10;

	this->is_destroyed = false;

	if (x > 240)
		this->move_to_left = true;
	else
		this->move_to_left = false;

	this->hitbox = {
		25, 0,
		32, 42
	};
	
	this->explosion = new Spritesheet(renderer->renderer, "assets/explosion.png", 142, 172);
	this->animation_counter = 0;
}

Enemy::~Enemy() {
	delete this->explosion;
}

void Enemy::Render(double dt) {
	this->pos.y += ENEMY_SPEED * dt;
	this->pos.x += this->x_speed * dt;

	this->x_speed += 10 * dt * (this->move_to_left ? -1 : 1);

	if (this->bullet_timer >= BULLET_COOLDOWN) {
		Bullet* bullet1 = new Bullet(this->sprite_renderer, (int)this->pos.x + 14, (int)this->pos.y + 25, { 0.25, -1 });
		Bullet* bullet2 = new Bullet(this->sprite_renderer, (int)this->pos.x + 67, (int)this->pos.y + 25, { -0.25, -1 });

		this->sprite_renderer->AddSprite(bullet1);
		this->sprite_renderer->AddSprite(bullet2);

		this->bullet_timer = 0;
	}
	this->bullet_timer += dt;

	if (this->is_destroyed) {
		SDL_Rect rect = {
			(int)this->pos.x, (int)this->pos.y,
			(int)this->size.w, (int)this->size.h
		};

		int frame = (int)this->animation_counter % ANIMATION_FRAME_COUNT;

		this->explosion->CopyTileTo(frame, 0, &rect);

		if (frame >= ANIMATION_FRAME_COUNT - 1)
			this->sprite_renderer->DestroySprite(this);

		this->animation_counter += ANIMATION_FPS * dt;
		return;
	}

	Sprite::Render(dt);

	if (this->pos.y >= 640)
		this->sprite_renderer->DestroySprite(this);
}

void Enemy::OnCollision(Sprite* collided) {
	if (dynamic_cast<const Bullet*>(collided) != nullptr && !this->is_destroyed) {
		this->sprite_renderer->DestroySprite(collided);
		this->hp--;

		if (this->hp == 0) {
			this->is_destroyed = true;
			Game::player->score += 50;
		}
	}
}