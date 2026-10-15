#ifndef SPRITESHEET_HPP
#define SPRITESHEET_HPP

#include <SDL.h>
#include <unordered_map>
#include "types.hpp"


class Spritesheet {
private:
	static std::unordered_map<const char*, SDL_Texture*> spritesheet_cache;

	types::Vector2d tileSize;
	SDL_Texture* texture;
	SDL_Renderer* renderer;
	char* path;

public:
	Spritesheet(SDL_Renderer* renderer, const char* path, int tileW, int tileH);
	~Spritesheet();

	SDL_Texture* GetTextureAt(int x, int y);
	void CopyTo(int x, int y, SDL_Rect* dstrect);
};

#endif