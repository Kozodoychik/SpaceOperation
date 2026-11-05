#pragma once

#include <SDL.h>

const inline Uint32 EVENT_GAMEOVER{ SDL_RegisterEvents(1) };
const inline Uint32 EVENT_COLLISION{ SDL_RegisterEvents(1) };
const inline Uint32 EVENT_SCORE_ADD{ SDL_RegisterEvents(1) };
const inline Uint32 EVENT_PLAYER_DAMAGE{ SDL_RegisterEvents(1) };

const inline Uint32 EVENT_GAME_FREEZE{ SDL_RegisterEvents(1) };
const inline Uint32 EVENT_GAME_UNFREEZE{ SDL_RegisterEvents(1) };
const inline Uint32 EVENT_GAME_RESTART{ SDL_RegisterEvents(1) };
const inline Uint32 EVENT_GAME_RESET{ SDL_RegisterEvents(1) };