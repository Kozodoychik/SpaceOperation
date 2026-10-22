#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SDL.h>
#include "sprite.hpp"

typedef union {
	struct {
		bool w : 1;
		bool a : 1;
		bool s : 1;
		bool d : 1;
		bool space : 1;
	};
	char flags;
} KeyMap;

class Player : public Sprite {
private:
	double bullet_timer;
	double hit_timer;

	bool is_moving = false;
	KeyMap key_map;

public:
	int hp;
	int score;

	Player(SpriteRenderer* renderer);

	void Render(double dt) override;
	void OnEvent(SDL_Event e) override;
	void OnCollision(Sprite* collided) override;
};

#endif