#include "spritesheet.hpp"
#include "logging.hpp"
#include <SDL.h>
#include <SDL_image.h>
#include <format>

std::unordered_map<const char*, SDL_Texture*> Spritesheet::spritesheet_cache;

Spritesheet::Spritesheet(SDL_Renderer* renderer, const char* path, int tileW, int tileH) {
	this->renderer = renderer;

	auto it = this->spritesheet_cache.find(path);

	if (it != this->spritesheet_cache.end()) {
		this->texture = it->second;
	}
	else
	{
		logging::Info(MODULE_NAME, std::format("Loading spritesheet from file: {} (tileW: {}, tileH: {})", path, tileW, tileH).c_str());

		this->texture = IMG_LoadTexture(renderer, path);

		if (!texture) {
			this->path = (char*)"DUMMY TEXUTRE!!!";

			logging::Warn(MODULE_NAME, std::format("Cannot load spritesheet texture: {}. Generating dummy texture", IMG_GetError()).c_str());

			this->tileSize.w = 64;
			this->tileSize.h = 64;

			this->texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 64, 64);

			SDL_SetRenderTarget(renderer, this->texture);
			SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
			SDL_RenderClear(renderer);
			SDL_SetRenderTarget(renderer, NULL);

			return;
		}

		this->spritesheet_cache[path] = this->texture;
	}

	SDL_SetTextureBlendMode(this->texture, SDL_BLENDMODE_BLEND);

	this->tileSize.w = tileW;
	this->tileSize.h = tileH;

	this->path = (char*)path;

}

Spritesheet::~Spritesheet() {

	//logging::Info(MODULE_NAME, std::format("Destroying spritesheet: {}", this->path).c_str());

	//SDL_DestroyTexture(this->texture);

}

SDL_Texture* Spritesheet::GetTextureAt(int x, int y) {

	SDL_Texture* texture = SDL_CreateTexture(
		this->renderer,
		SDL_PIXELFORMAT_RGBA8888,
		SDL_TEXTUREACCESS_TARGET,
		(int)this->tileSize.w,
		(int)this->tileSize.h
	);

	SDL_Rect dstRect = {
		0, 0,
		(int)this->tileSize.w, (int)this->tileSize.h
	};

	SDL_Rect srcRect = {
		(int)this->tileSize.w * x, (int)this->tileSize.h * y,
		(int)this->tileSize.w, (int)this->tileSize.h
	};

	SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
	SDL_SetRenderTarget(this->renderer, texture);
	SDL_RenderCopy(this->renderer, this->texture, &srcRect, &dstRect);
	SDL_SetRenderTarget(this->renderer, NULL);

	return texture;
}

void Spritesheet::CopyTo(int x, int y, SDL_Rect* dstRect) {
	SDL_Rect srcRect = {
		(int)this->tileSize.w * x, (int)this->tileSize.h * y,
		(int)this->tileSize.w, (int)this->tileSize.h
	};
	SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
	SDL_RenderCopy(this->renderer, this->texture, &srcRect, dstRect);
}