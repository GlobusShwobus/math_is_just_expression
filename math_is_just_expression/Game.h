#pragma once

#include "Entity.h"

class Game {

	sf::RenderWindow window;

	sf::Text score_text;

	bool running = true;
	bool paused = false;
	size_t current_frame = 0;

	EntityManager entities;
	Player player;

	nlohmann::json& config;

public:

	Game(nlohmann::json& CONFIG) :config(CONFIG), entities(CONFIG), player(CONFIG) {}

	void Init() {
		//read from teh config json, the window one and set up window size, name, framrate

		window.create(sf::VideoMode(1280, 720), "TEST123");
		window.setFramerateLimit(60);


		//window.create(sf::Videomode(x,y),"name")
		//window.setFramerateLimit(xyz)

		//spawn player
	}
	void Run() {
		//add pause, some sys should work while paused, others not


		while (running) {

			entities.Update();

			if (!paused) {
				sEnemySpawner();
				sMovement();
				sCollision();
			}
			sUserInput();
			sRender();

			current_frame++;
		}

	}
	void SetPaused() {
		paused = !paused;
	}

	void sEnemySpawner() {
		//entity manager is not set upp all the way to support this, but essentially call addEntity function
	}

	void sRender() {

		window.clear();


		player.shape.rect.setPosition(player.transform.pos.x, player.transform.pos.y);

		//here draw other shit too
		
		//for (const auto& each : entities.GetEntities()) {
		//	window.draw(each.shape.rect);
		//}

		window.draw(player.shape.rect);
		window.display();
	}

	void sUserInput() {

		sf::Event sfevent;

		while (window.pollEvent(sfevent)) {

			//closes game
			if (sfevent.type == sf::Event::Closed) {
				running = false;
			}

			if (sfevent.type == sf::Event::KeyPressed) {
				switch (sfevent.key.code) {
				case sf::Keyboard::W: player.input.up    = true; break;
				case sf::Keyboard::A: player.input.left  = true; break;
				case sf::Keyboard::S: player.input.down  = true; break;
				case sf::Keyboard::D: player.input.right = true; break;
				default: /*jack shit i think???*/ break;
				}
			}
			if (sfevent.type == sf::Event::KeyReleased) {
				switch (sfevent.key.code) {
			    case sf::Keyboard::W: player.input.up    = false; break;
				case sf::Keyboard::A: player.input.left  = false; break;
				case sf::Keyboard::S: player.input.down  = false; break;
				case sf::Keyboard::D: player.input.right = false; break;
				default: /*jack shit i think???*/ break;
				}
			}

			if (sfevent.type == sf::Event::MouseButtonPressed) {
				if (sfevent.key.code == sf::Mouse::Left) {
					player.input.shoot = true;
					//also spawn bullet here
					//handle spawning bullet somewhere
				}
				if (sfevent.key.code == sf::Mouse::Right) {
					player.input.shield = true;
					//also spawn bullet here
					//handle spawning shield somewhere
				}
			}
			if (sfevent.type == sf::Event::MouseButtonReleased) {
				if (sfevent.key.code == sf::Mouse::Left) {
					player.input.shoot = false;
					//don't shoot any longer, (what about holding key???)
				}
				if (sfevent.key.code == sf::Mouse::Right) {
					player.input.shield = false;
					//(think deactivate shield is fine as long as it is not destroyed, meaning mem is not released something something we'll see)
				}
			}
		}

	}

	void sMovement() {
		//also make sure everything else that moves, moves
		//think that's it tho
		for (auto& each : entities.GetEntities()) {
			each.transform.pos += each.transform.vel;
		}

		player.transform.vel = { 0,0 };		//reset vel each frame

		if (player.input.up) {
			player.transform.vel.y -= player.transform.speed;
		}
		if (player.input.left) {
			player.transform.vel.x -= player.transform.speed;
		}
		if (player.input.down) {
			player.transform.vel.y += player.transform.speed;
		}
		if (player.input.right) {
			player.transform.vel.x += player.transform.speed;
		}



		player.transform.pos += player.transform.vel;
	}

	void sCollision() {
		for (auto& each : entities.GetEntities()) {

		}
	}


};
