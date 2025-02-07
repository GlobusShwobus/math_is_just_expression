#include <fstream>
#include "RenderWindow.h"


class JHandle {
    nlohmann::json* json= nullptr;
public:

    const nlohmann::json* const Get()const {
        return json;
    }
    bool Good()const {
        return json;
    }

    JHandle(const char* filename) {
        std::ifstream in(filename);
        if (in.good()) {
            json = new nlohmann::json();
            in >> *json;
        }
    }

    ~JHandle() {
        if (json) {
            delete json;
        }
    }
};

struct WindowInit {

    std::string title;
    unsigned int width = 0;
    unsigned int height = 0;

    WindowInit(const nlohmann::json& json) {

        auto& wStuff = json["Window"];

        title = wStuff["game_name"];
        width = wStuff["width"];
        height = wStuff["height"];
    }
};


int main() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    JHandle config("Config.json");

    if (!config.Good()) {
        MessageBox(nullptr, L"Config failed init", L"error init", MB_OK | MB_ICONERROR);
        return -1;
    }
    
    WindowInit wParams(*config.Get());


    if (!SDL_Init(SDL_INIT_VIDEO)) {
        MessageBox(nullptr, L"SDL init fail", L"error init", MB_OK | MB_ICONERROR);
        return -1;
    }

    RenderWindow window(wParams.title.c_str(), wParams.width, wParams.height);

    if (!window.Good()) {
        MessageBox(nullptr, L"Window failed init", L"error init", MB_OK | MB_ICONERROR);
        return -1;
    }

    bool gameRunning = true;
    
    SDL_Event event;

    while (gameRunning) {

        SDL_PollEvent(&event);

        if (event.type == SDL_EventType::SDL_EVENT_QUIT) {
            gameRunning = false;
        }

    }


    window.~RenderWindow();
    SDL_Quit();

    return 0;
}
