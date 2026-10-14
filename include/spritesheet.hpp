#include <SDL.h>
#include "types.hpp"


class Spritesheet {
private:
	types::Vector2d tileSize;
	SDL_Texture* texture;
	SDL_Renderer* renderer;
	char* path;

public:
	Spritesheet(SDL_Renderer* renderer, const char* path, int tileW, int tileH);
	~Spritesheet();

	SDL_Texture* GetTextureAt(int x, int y);
};

/*class Tile {
private:

public:

	Tile();
	~Tile();

	SDL_Texture* GetTexture();

};*/