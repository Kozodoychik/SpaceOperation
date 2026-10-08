#include <iostream>
#include "game.hpp"
#include "logging.hpp"

int main(int argc, char* argv[])
{

    Game* game = new Game();
    int status = game->Initialize();

    if (status) return status;

    game->MainLoop();
    

    return 0;

}
