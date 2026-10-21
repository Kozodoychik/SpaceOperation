#include "sprite.hpp"
#include "types.hpp"


class Bullet : public Sprite {
private:
	types::Vector2d direction;

public:
	Bullet(SpriteRenderer* renderer, int x, int y, types::Vector2d direction);

	void Render(double dt) override;
};