#include "sprite.hpp"

class Bullet : public Sprite {
public:
	Bullet(SpriteRenderer* renderer, int x, int y);

	void Render(double dt) override;
};