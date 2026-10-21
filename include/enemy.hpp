#include "sprite.hpp"
#include "spritesheet.hpp"

class Enemy : public Sprite {
private:
	double bullet_timer;
	double x_speed;
	double animation_counter;
	int hp;

	bool move_to_left;
	bool is_destroyed;

	Spritesheet* explosion;

public:
	Enemy(SpriteRenderer* renderer, int x, int y);
	~Enemy();

	void Render(double dt) override;
	void OnCollision(Sprite* collided) override;
};