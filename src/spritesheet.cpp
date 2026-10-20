#include "spritesheet.hpp"
#include "logging.hpp"
#include <SDL.h>
#include <SDL_image.h>
#include <format>

std::unordered_map<const char*, SDL_Texture*> Spritesheet::spritesheet_cache;

Spritesheet::Spritesheet(SDL_Renderer* renderer, const char* path, int tileW, int tileH) {
	this->renderer = renderer;
	this->path = (char*)path;

	auto it = this->spritesheet_cache.find(path);
	  
	if (it != this->spritesheet_cache.end()) {
		this->texture = it->second;
	}
	else
	{
		logging::Info(MODULE_NAME, std::format("Loading spritesheet from file: {} (tileW: {}, tileH: {})", path, tileW, tileH).c_str());

		this->texture = IMG_LoadTexture(renderer, path);

		if (!texture) {
			logging::Warn(MODULE_NAME, std::format("Cannot load spritesheet texture: {}. Generating dummy texture", IMG_GetError()).c_str());

			this->texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, tileW, tileH);

			SDL_SetRenderTarget(renderer, this->texture);

			// Генерируем текстуру розовой "шахматной доски"
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			SDL_RenderClear(renderer);
			
			SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
			SDL_Rect upLeft = { 0, 0, tileW / 2, tileH / 2 };
			SDL_Rect downRight = { tileW / 2, tileH / 2, tileW / 2 + 1, tileH / 2 + 1 };
			SDL_RenderFillRect(renderer, &upLeft);
			SDL_RenderFillRect(renderer, &downRight);

			SDL_SetRenderTarget(renderer, NULL);
		}

		this->spritesheet_cache[path] = this->texture;
	}

	SDL_SetTextureBlendMode(this->texture, SDL_BLENDMODE_BLEND);

	this->tileSize.w = tileW;
	this->tileSize.h = tileH;

	int sheetWidth, sheetHeight;
	SDL_QueryTexture(this->texture, NULL, NULL, &sheetWidth, &sheetHeight);

	this->imageSize.w = (double)sheetWidth;
	this->imageSize.h = (double)sheetHeight;
}

void Spritesheet::CopyTileTo(int tileX, int tileY, SDL_Rect* dstRect) {
	int offsetX = (int)this->tileSize.w * tileX;
	int offsetY = (int)this->tileSize.h * tileY;

	if (offsetX >= this->imageSize.w)
		offsetX -= (int)this->imageSize.w * (int)(offsetX / this->imageSize.w);

	if (offsetY >= this->imageSize.h)
		offsetY -= (int)this->imageSize.h * (int)(offsetY / this->imageSize.h);

	SDL_Rect srcRect = {
		offsetX, offsetY,
		(int)this->tileSize.w, (int)this->tileSize.h
	};

	SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
	SDL_RenderCopy(this->renderer, this->texture, &srcRect, dstRect);
}

void Spritesheet::CopyTileToEx(int tileX, int tileY, double rotation, SDL_Point* center, SDL_Rect* dstRect, SDL_RendererFlip flip) {
	int offsetX = (int)this->tileSize.w * tileX;
	int offsetY = (int)this->tileSize.h * tileY;

	if (offsetX >= this->imageSize.w)
		offsetX -= (int)this->imageSize.w * (int)(offsetX / this->imageSize.w);

	if (offsetY >= this->imageSize.h)
		offsetY -= (int)this->imageSize.h * (int)(offsetX / this->imageSize.h);

	SDL_Rect srcRect = {
		offsetX, offsetY,
		(int)this->tileSize.w, (int)this->tileSize.h
	};

	SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
	SDL_RenderCopyEx(this->renderer, this->texture, &srcRect, dstRect, rotation, center, flip);
}

void Spritesheet::ClearCache() {
	for (auto& it : Spritesheet::spritesheet_cache) {
		logging::Info(MODULE_NAME, std::format("Clearing spritesheet cache: Destroying: {}", it.first).c_str());
		SDL_DestroyTexture(it.second);
	}
}