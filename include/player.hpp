#include <SDL.h>
#include "sprite.hpp"

typedef struct {
	bool w : 1;
	bool a : 1;
	bool s : 1;
	bool d : 1;
} KeyMapBool;

typedef union {
	KeyMapBool keys;
	int flags;
} KeyMap;

class Player : public Sprite {
private:
	bool is_moving = false;
	KeyMap key_map;

public:
	Player(SDL_Renderer* renderer);
	~Player();

	void Render(double dt) override;
	void OnEvent(SDL_Event e) override;
};