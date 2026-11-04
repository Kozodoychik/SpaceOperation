#pragma once

#include <SDL.h>

const inline Uint32 EVENT_GAMEOVER{ SDL_RegisterEvents(1) };
const inline Uint32 EVENT_COLLISION{ SDL_RegisterEvents(1) };
const inline Uint32 EVENT_SCORE_ADD{ SDL_RegisterEvents(1) };