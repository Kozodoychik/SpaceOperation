#include <SDL.h>
#include "sprite.hpp"
#include "spritesheet.hpp"


class Enemy : public Sprite {
private:
	
	typedef enum {

		ALIVE,
		DESTROYED

	} EnemyState;

	double bullet_timer;
	double x_speed;
	double animation_counter;
	int hp;

	bool move_to_left;
	
	EnemyState state;

	Spritesheet* explosion;

public:
	Enemy(SpriteRenderer* renderer, int x, int y);
	~Enemy();

	void Render(double dt) override;
	void OnEvent(SDL_Event e) override;
};