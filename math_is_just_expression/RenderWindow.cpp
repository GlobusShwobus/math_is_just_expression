#include "RenderWindow.h"

RenderWindow::RenderWindow(const char* title, int width, int height) {
	
	window = SDL_CreateWindow(title, width, height, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);

	if (window) {
		renderer = SDL_CreateRenderer(window, NULL);
	}

}