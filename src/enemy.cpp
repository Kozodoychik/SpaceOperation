#include "enemy.hpp"
#include "sprite.hpp"
#include "bullet.hpp"
#include "game.hpp"
#include "events.hpp"
#include "logging.hpp"

#define ENEMY_SPEED 150

#define BULLET_COOLDOWN 0.5

#define ANIMATION_FPS 32
#define ANIMATION_FRAME_COUNT 17

Enemy::Enemy(SpriteRenderer* renderer, int x, int y) : Sprite(renderer, "assets/enemy.png", x, y, 82, 42) {
	this->rotation_angle = 180;
	this->bullet_timer = 0;
	this->hp = 4;
	this->x_speed = 10;

	this->state = ALIVE;

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

	switch (this->state) {
	case ALIVE: {
		Sprite::Render(dt);

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

void Enemy::OnEvent(SDL_Event e) {
	if (this->state == DESTROYED) return;

	if (e.type == EVENT_COLLISION) {
		Sprite* collided = (Sprite*)e.user.data2;

        if ((dynamic_cast<const Bullet*>(collided) != nullptr)) {
			this->sprite_renderer->DestroySprite(collided);
			this->hp--;

			if (this->hp == 0) {
				this->state = DESTROYED;

				SDL_Event e{ EVENT_SCORE_ADD };
				e.user.code = 250;
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