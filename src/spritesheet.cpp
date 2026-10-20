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

void Spritesheet::CopyTileTo(int tileX, int tileY, SDL_Rect* dstRect) {
	SDL_Rect srcRect = {
		(int)this->tileSize.w * tileX, (int)this->tileSize.h * tileY,
		(int)this->tileSize.w, (int)this->tileSize.h
	};
	SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
	SDL_RenderCopy(this->renderer, this->texture, &srcRect, dstRect);
}

void Spritesheet::CopyTileToEx(int tileX, int tileY, double rotation, SDL_Point* center, SDL_Rect* dstRect, SDL_RendererFlip flip) {
	SDL_Rect srcRect = {
		(int)this->tileSize.w * tileX, (int)this->tileSize.h * tileY,
		(int)this->tileSize.w, (int)this->tileSize.h
	};

	SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
	SDL_RenderCopyEx(this->renderer, this->texture, &srcRect, dstRect, rotation, center, flip);
}