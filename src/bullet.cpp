#include "bullet.hpp"
#include "sprite.hpp"

Bullet::Bullet(SpriteRenderer* renderer, int x, int y) : Sprite(renderer, "assets/bullet.png", x, y, 5, 16) {}

Bullet::~Bullet() {}

void Bullet::Render(double dt) {
	Sprite::Render(dt);

	this->pos.y -= 800 * dt;

	if (this->pos.y <= 0) {
		this->sprite_renderer->DestroySprite(this);
	}
}