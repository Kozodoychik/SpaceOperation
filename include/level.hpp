#include <SDL.h>
#include "sprite.hpp"
#include "spritesheet.hpp"
#include "player.hpp"


class LevelManager {
private:

	SDL_Renderer* renderer;
	SpriteRenderer* sprite_renderer;

	Spritesheet* background;
	double bg_scroll;
	
public:
	Player* player;

	int difficulty;

	LevelManager(SDL_Renderer* renderer);
	~LevelManager();

	void GenerateObstacles();
	void Reset();

	void Render(double dt);
	void HandleEvent(SDL_Event e);
	void RenderBackground(double dt);
};