#include <SDL.h>
#include "sprite.hpp"
#include "spritesheet.hpp"
#include "logging.hpp"


Sprite::Sprite(SDL_Renderer* renderer, const char* texture_file, double x, double y, double w, double h) {

	this->renderer = renderer;

	Spritesheet* spritesheet = new Spritesheet(renderer, "assets/spaceship.png", (int)w, (int)h);

	this->texture = spritesheet->GetTextureAt(0, 0);
	this->pos = { x, y };
	this->size = { w, h };

	delete spritesheet;

}

Sprite::~Sprite() {
	logging::Info(MODULE_NAME, "Destroying sprite");

	SDL_DestroyTexture(this->texture);
}

void Sprite::Render(double dt) {

	SDL_Rect rect = {
		(int)this->pos.x, (int)this->pos.y,
		(int)this->size.w, (int)this->size.h
	};

	SDL_RenderCopy(this->renderer, this->texture, NULL, &rect);
	//SDL_RenderPresent(this->renderer);

}

void Sprite::OnEvent(SDL_Event e) {

}

void Sprite::OnCollision(Sprite* collided) {
	logging::Warn(MODULE_NAME, "OnCollision: not implemented yet");
}