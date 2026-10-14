#ifndef SPRITE_HPP
#define SPRITE_HPP

#include <SDL.h>
#include "types.hpp"


class Sprite {
protected:
	SDL_Renderer* renderer;
	SDL_Texture* texture;
	types::Vector2d pos = { 0, 0 };
	types::Vector2d size = { 0, 0 };

public:
	Sprite(SDL_Renderer* renderer, const char* texture_file, double x, double y, double w, double h);
	~Sprite();

	virtual void Render(double dt);
	virtual void OnEvent(SDL_Event e);
	virtual void OnCollision(Sprite* collided);
};

#endif