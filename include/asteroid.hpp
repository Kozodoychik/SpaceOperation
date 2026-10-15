#include "sprite.hpp"

class Asteroid : public Sprite {
private:
	double acceleration;
	double angle;
	double rotation_speed;
	int hp;
	int size_type;

public:
	Asteroid(SpriteRenderer* renderer, int x, int y);

	void Render(double dt) override;
	void OnCollision(Sprite* collided) override;
};