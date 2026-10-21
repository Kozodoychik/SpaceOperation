#ifndef SPRITESHEET_HPP
#define SPRITESHEET_HPP

#include <SDL.h>
#include <unordered_map>
#include "types.hpp"


class Spritesheet {
private:
	types::Vector2d tileSize;
	types::Vector2d imageSize;

	SDL_Texture* texture;
	SDL_Renderer* renderer;
	char* path;

public:
	static std::unordered_map<const char*, SDL_Texture*> spritesheet_cache;

	Spritesheet(SDL_Renderer* renderer, const char* path, int tileW, int tileH);

	void CopyTileTo(int tileX, int tileY, SDL_Rect* dstrect);
	void CopyTileToEx(int tileX, int tileY, double rotation, SDL_Point* center, SDL_Rect* dstRect, SDL_RendererFlip flip);

	static void ClearCache();
};

#endif