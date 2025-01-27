

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


    Game lolw123(config);
    lolw123.Init();
    lolw123.Run();

    return 0;
}