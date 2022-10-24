#include <iostream>
#include "Game.h"
#include "Frame.h"

enum difficulty {
    EASY = 0,
    MEDIUM = 2,
    IMPOSSIBLE = 5
};

int main(int argc, char* argv[])
{
    Game *game = new Game(difficulty::IMPOSSIBLE);
    game->start();

    return 0;
}









