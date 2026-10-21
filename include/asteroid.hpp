#include "sprite.hpp"
#include "spritesheet.hpp"

class Asteroid : public Sprite {
private:
	double acceleration;
	double rotation_speed;
	double animation_counter;
	int hp;
	int size_type;

	bool is_destroyed;

	Spritesheet* explosion;

public:
	Asteroid(SpriteRenderer* renderer, int x, int y);
	~Asteroid();

	void Render(double dt) override;
	void OnCollision(Sprite* collided) override;
};