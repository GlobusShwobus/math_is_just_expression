#pragma once

#include "Entity.h"

/*


set up the main rendering window in game::init, read window parameters from the config

*/


class Game {

	sf::RenderWindow window;
	EntityManager entities;
	sf::Text score_text;

public:

	void Init() {
		//read from teh config json, the window one and set up window size, name, framrate
	}
	void Run() {

	}
};
