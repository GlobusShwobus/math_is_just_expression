

#include "Game.h"
#include <fstream>


/*
finish up basic skeleton code first, leaning towards no random spawns, collision and bullet spawning mainly needs a look

set up the window from config

set up a 2D array for making the screen into a grid

set up a basic scene, think some shit like pac man

test using chat gpt API for idk, something small

*/

int main() {

    std::ifstream in("Config.json");

    nlohmann::json config;

    if (in.good()) {
        in >> config;
    }
    else {
        return -1;
    }

    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    Game lolw123(config);
    lolw123.Init();
    lolw123.Run();

    return 0;
}