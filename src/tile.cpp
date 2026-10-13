#include "tile.hpp"
#include "logging.hpp"
#include <SDL.h>
#include <SDL_image.h>
#include <format>


Spritesheet::Spritesheet(SDL_Renderer* renderer, const char* path, int tileW, int tileH) {

	logging::Info(MODULE_NAME, std::format("Loading spritesheet from file: {} (tileW: {}, tileH: {})", path, tileW, tileH).c_str());

	this->renderer = renderer;
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

	this->tileSize.w = tileW;
	this->tileSize.h = tileH;

	this->path = (char*)path;

}

Spritesheet::~Spritesheet() {

	logging::Info(MODULE_NAME, std::format("Destroying spritesheet: {}", this->path).c_str());

	SDL_DestroyTexture(this->texture);

}

SDL_Texture* Spritesheet::GetTextureAt(int x, int y) {

	SDL_Texture* texture = SDL_CreateTexture(
		this->renderer,
		SDL_PIXELFORMAT_RGBA8888,
		SDL_TEXTUREACCESS_TARGET,
		this->tileSize.w,
		this->tileSize.h
	);

	SDL_Rect dstRect = {
		0, 0,
		this->tileSize.w, this->tileSize.h
	};

	SDL_Rect srcRect = {
		this->tileSize.w * x, this->tileSize.h * y,
		this->tileSize.w, this->tileSize.h
	};

	SDL_SetRenderTarget(this->renderer, texture);
	SDL_RenderCopy(this->renderer, this->texture, &srcRect, &dstRect);
	SDL_SetRenderTarget(this->renderer, NULL);

	return texture;
}