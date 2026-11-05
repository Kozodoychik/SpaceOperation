#include <SDL.h>
#include "sprite.hpp"
#include "spritesheet.hpp"
#include "events.hpp"


Sprite::Sprite(SpriteRenderer* renderer, const char* texture_file, double x, double y, double w, double h) {

	this->sprite_renderer = renderer;

	this->spritesheet = new Spritesheet(this->sprite_renderer->renderer, texture_file, (int)w, (int)h);

	this->pos = { x, y };
	this->size = { w, h };
	this->rotation_angle = 0;

	this->hitbox = {
		0, 0,
		(int)w, (int)h
	};
}

Sprite::~Sprite() {
	delete this->spritesheet;
}

void Sprite::Render(double dt) {

	SDL_Rect rect = {
		(int)this->pos.x, (int)this->pos.y,
		(int)this->size.w, (int)this->size.h
	};
	
	SDL_Point center = { (int)this->size.w / 2, (int)this->size.h / 2 };

	this->spritesheet->CopyTileToEx(0, 0, this->rotation_angle, &center, &rect, SDL_FLIP_NONE);

}

void Sprite::OnEvent(SDL_Event e) {}

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

	std::size_t count = this->sprites.size();

	for (int i = 0; i < count; i++) {
		this->sprites[i]->Render(dt);
	}

	for (int spriteA = 0; spriteA < count; spriteA++) {
		SDL_Rect a = {
			(int)this->sprites[spriteA]->pos.x + this->sprites[spriteA]->hitbox.x, 
			(int)this->sprites[spriteA]->pos.y + this->sprites[spriteA]->hitbox.y,
			
			(int)this->sprites[spriteA]->hitbox.w, 
			(int)this->sprites[spriteA]->hitbox.h
		};

		for (int spriteB = 0; spriteB < count; spriteB++) {
			if (spriteA == spriteB) continue;

			SDL_Rect b = {
				(int)this->sprites[spriteB]->pos.x + this->sprites[spriteB]->hitbox.x,
				(int)this->sprites[spriteB]->pos.y + this->sprites[spriteB]->hitbox.y,

				(int)this->sprites[spriteB]->hitbox.w,
				(int)this->sprites[spriteB]->hitbox.h
			};

			if (SDL_HasIntersection(&a, &b)) {
				SDL_Event e = { EVENT_COLLISION };

				e.user.data1 = this->sprites[spriteA];
				e.user.data2 = this->sprites[spriteB];

				SDL_PushEvent(&e);
			}
		}
	}
}

void SpriteRenderer::HandleEvent(SDL_Event e) {
	std::size_t count = this->sprites.size();

	for (int i = 0; i < count; i++) {
		if ((e.type == EVENT_COLLISION && e.user.data1 == this->sprites[i]) || e.type != EVENT_COLLISION)
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