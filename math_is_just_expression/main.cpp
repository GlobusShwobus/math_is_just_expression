

#include "Game.h"
#include <fstream>

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