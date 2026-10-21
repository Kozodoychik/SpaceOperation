#ifndef SPRITE_HPP
#define SPRITE_HPP

#include <SDL.h>
#include <vector>
#include <memory>
#include "types.hpp"
#include "spritesheet.hpp"

class SpriteRenderer;

class Sprite {
protected:
	SpriteRenderer* sprite_renderer;
	Spritesheet* spritesheet;

public:
	double rotation_angle;

	types::Vector2d pos = { 0, 0 };
	types::Vector2d size = { 0, 0 };

	SDL_Rect hitbox;

	Sprite(SpriteRenderer* renderer, const char* texture_file, double x, double y, double w, double h);
	~Sprite();

	virtual void Render(double dt);
	virtual void OnEvent(SDL_Event e);
	virtual void OnCollision(Sprite* collided);
};

class SpriteRenderer {
private:
	std::vector<Sprite*> sprites;
	std::vector<Sprite*> to_destroy;

public:
	SDL_Renderer* renderer;

	SpriteRenderer(SDL_Renderer* renderer);
	~SpriteRenderer();

	void Render(double dt);
	void HandleEvent(SDL_Event e);
	
	void AddSprite(Sprite* sprite);
	void DestroySprite(Sprite* sprite);
};

#endif