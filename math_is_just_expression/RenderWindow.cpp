#include "RenderWindow.h"

RenderWindow::RenderWindow(const char* title, int width, int height) {
	
	window = SDL_CreateWindow(title, width, height, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);

	if (window) {
		renderer = SDL_CreateRenderer(window, NULL);
	}

}

SDL_Texture* RenderWindow::LoadTexture(const char* filepath) {
	SDL_Texture* texture = nullptr;

	texture = IMG_LoadTexture(renderer, filepath);

	//check somewhere else
	//if (!texture) {
	//	std::string err = "Failed to load texture: " + std::string(filepath);
	//	throw std::exception(err.c_str());
	//}

	return texture;
}