#include <SDL.h>
#include "sprite.hpp"
#include "spritesheet.hpp"
#include "logging.hpp"


Sprite::Sprite(SpriteRenderer* renderer, const char* texture_file, double x, double y, double w, double h) {

	this->sprite_renderer = renderer;

	Spritesheet* spritesheet = new Spritesheet(this->sprite_renderer->renderer, texture_file, (int)w, (int)h);

	this->texture = spritesheet->GetTextureAt(0, 0);
	this->pos = { x, y };
	this->size = { w, h };

	delete spritesheet;

}

Sprite::~Sprite() {
	SDL_DestroyTexture(this->texture);
}

void Sprite::Render(double dt) {

	SDL_Rect rect = {
		(int)this->pos.x, (int)this->pos.y,
		(int)this->size.w, (int)this->size.h
	};

	SDL_RenderCopy(this->sprite_renderer->renderer, this->texture, NULL, &rect);

}

void Sprite::OnEvent(SDL_Event e) {

}

void Sprite::OnCollision(Sprite* collided) {

}

SpriteRenderer::SpriteRenderer(SDL_Renderer* renderer) {
	this->renderer = renderer;
}

SpriteRenderer::~SpriteRenderer() {
	for (Sprite* sprite : this->sprites) {
		delete sprite;
	}
}

void SpriteRenderer::Render(double dt) {

	for (Sprite* sprite : this->to_destroy) {
		auto it = std::find(this->sprites.begin(), this->sprites.end(), sprite);

		if (it != this->sprites.end()) {
			this->sprites.erase(it);
			delete sprite;
		}
	}
	this->to_destroy.clear();

	int count = this->sprites.size();

	for (int i = 0; i < count; i++) {
		this->sprites[i]->Render(dt);
	}

	for (int spriteA = 0; spriteA < count; spriteA++) {
		SDL_Rect a = {
			this->sprites[spriteA]->pos.x, this->sprites[spriteA]->pos.y,
			this->sprites[spriteA]->size.w, this->sprites[spriteA]->size.h
		};

		for (int spriteB = 0; spriteB < count; spriteB++) {
			if (spriteA == spriteB) continue;

			SDL_Rect b = {
				this->sprites[spriteB]->pos.x, this->sprites[spriteB]->pos.y,
				this->sprites[spriteB]->size.w, this->sprites[spriteB]->size.h
			};

			if (SDL_HasIntersection(&a, &b)) {
				this->sprites[spriteA]->OnCollision(this->sprites[spriteB]);
				this->sprites[spriteB]->OnCollision(this->sprites[spriteA]);
			}
		}
	}
}

void SpriteRenderer::HandleEvent(SDL_Event e) {
	int count = this->sprites.size();

	for (int i = 0; i < count; i++) {
		this->sprites[i]->OnEvent(e);
	}
}

void SpriteRenderer::AddSprite(Sprite* sprite) {
	this->sprites.push_back(sprite);
}

void SpriteRenderer::DestroySprite(Sprite* sprite) {
	auto it = this->sprites.begin();

	it = std::find(this->sprites.begin(), this->sprites.end(), sprite);
	
	if (it != this->sprites.end()) {
		this->to_destroy.push_back(sprite);
	}
}