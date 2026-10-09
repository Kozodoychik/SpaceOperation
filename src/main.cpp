#include "game.hpp"
#include "logging.hpp"

int main(int argc, char* argv[])
{
    logging::OpenLogFile();

    Game* game = new Game();
    int status = game->Initialize();

    if (status) {
        logging::CloseLogFile();
        return status;
    }

    game->MainLoop();

    logging::CloseLogFile();
    
    return 0;

}
