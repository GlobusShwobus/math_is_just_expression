

#include "Game.h"
#include <fstream>
#include <conio.h>
/*
std::ifstream in("Config.json");

nlohmann::json config;

if (in.good()) {
    in >> config;
}
else {
    return -1;
}

std::srand(static_cast<unsigned int>(std::time(nullptr)));
*/

int main() {

    if (!SDL_InitSubSystem(SDL_Init(SDL_INIT_VIDEO))) {
        MessageBox(NULL, L"Big bad init fail", L"error", MB_OK | MB_ICONERROR);
    }

    auto window = SDL_CreateWindow("PEPEGA GAME", 800, 600, SDL_WINDOW_RESIZABLE);

    if (!window) {
        MessageBox(NULL, L"WINDOW FAILED", L"error", MB_OK | MB_ICONERROR);

    }

    bool test_run = true;

    while (test_run) {

        SDL_Event event;

        SDL_PollEvent(&event);

        if (event.type == SDL_EventType::SDL_EVENT_QUIT) {
            test_run = false;
        }

    }


    SDL_Quit();

    return 0;
}
