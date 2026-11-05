#include <cmath>
#include <numbers>
#include "bullet.hpp"
#include "sprite.hpp"
#include "events.hpp"
#include "player.hpp"

Bullet::Bullet(SpriteRenderer* renderer, int x, int y, types::Vector2d direction) : Sprite(renderer, "assets/bullet.png", x, y, 5, 16) {
	this->direction = direction;
}

void Bullet::Render(double dt) {
	this->pos.y -= 800 * dt * this->direction.y;
	this->pos.x -= 800 * dt * this->direction.x;

	this->rotation_angle = std::atan2(this->direction.y, this->direction.x) * (180 / std::numbers::pi) - 90;

	Sprite::Render(dt);

	if (this->pos.y <= 0 ||
		this->pos.y > 640 ||
		this->pos.x <= 0 ||
		this->pos.x > 640) {
		this->sprite_renderer->DestroySprite(this);
	}
}

void Bullet::OnEvent(SDL_Event e) {
	if (e.type == EVENT_COLLISION) {
		Sprite* collided = (Sprite*)e.user.data2;

		if (dynamic_cast<const Player*>(collided) != nullptr) {
			if (dynamic_cast<Player*>(collided)->hit_timer <= 0) {
				SDL_Event e{ EVENT_PLAYER_DAMAGE };
				e.user.code = 10;
				SDL_PushEvent(&e);

				this->sprite_renderer->DestroySprite(this);
			}
		}
	}
}