#include <SDL.h>
#include "sprite.hpp"
#include "spritesheet.hpp"


class Asteroid : public Sprite {
private:
	
	typedef enum {

		ALIVE,
		DESTROYED

	} AsteroidState;

	double acceleration;
	double rotation_speed;
	double animation_counter;
	int hp;
	int size_type;

	AsteroidState state;

	Spritesheet* explosion;

public:
	Asteroid(SpriteRenderer* renderer, int x, int y);
	~Asteroid();

	void Render(double dt) override;
	void OnEvent(SDL_Event e) override;
};