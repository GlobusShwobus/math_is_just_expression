#pragma once

#include "Entity.h"
#include <random>

class Game {

	sf::RenderWindow window;

	sf::Text score_text;

	bool running = true;
	bool paused = false;
	int64_t current_frame = 0;

	EntityManager entities;
	Player player;

	nlohmann::json& config;

	std::mt19937 rng;


public:

	Game(nlohmann::json& CONFIG) :config(CONFIG), entities(CONFIG), player(CONFIG, { 200,200 }), rng(std::random_device()()) {}

	void Init() {

		auto& sh = config["Window"];


		window.create(sf::VideoMode(sh["size_x"].get<int>(), sh["size_y"].get<int>()), sh["game_name"].get<std::string>());
		window.setFramerateLimit(sh["framerate_cap"].get<int>());

	}
	void Run() {

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


		
		static size_t last_spawn = 0;
		static std::uniform_int_distribution<> pos(0, 999);
		static std::uniform_int_distribution<>vel(-5, 5);
		
		if ((current_frame - last_spawn) >= 90) {
		
			entities.AddEnemy({ (float)pos(rng), (float)pos(rng) }, { (float)vel(rng), (float)vel(rng) });
			last_spawn = current_frame;
		}

	}

	void sRender() {

		window.clear();


		player.shape.rect.setPosition(player.transform.pos.x, player.transform.pos.y);

		//here draw other shit too
		
		for (auto& each : entities.GetEntities()) {
			each.shape.rect.setPosition(each.transform.pos.x, each.transform.pos.y);
			window.draw(each.shape.rect);
		}

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
				case sf::Keyboard::S: player.input.down = false; break;
				case sf::Keyboard::D: player.input.right = false; break;
				default: /*jack shit i think???*/ break;
				}
			}

			if (sfevent.type == sf::Event::MouseButtonPressed) {
				if (sfevent.key.code == sf::Mouse::Left) {
					player.input.shoot = true;

					sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

					entities.AddBullet({ player.transform.pos }, mousePos);
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
		std::vector<Entity*>bullets;
		std::vector<Entity*>enemies;

		for (auto& each : entities.GetEntities()) {

			float size_x = each.shape.rect.getSize().x;
			float size_y = each.shape.rect.getSize().y;

			//for left and right edges
			if (each.transform.pos.x < 0) {
				each.transform.pos.x = 0;
				each.transform.vel.x *= -1;
			}
			else if (each.transform.pos.x + size_x > window.getSize().x) {
				each.transform.pos.x = window.getSize().x - size_x;
				each.transform.vel.x *= -1;
			}
			//for up and down edges
			if (each.transform.pos.y < 0) {
				each.transform.pos.y = 0;
				each.transform.vel.y *= -1;
			}
			else if (each.transform.pos.y + size_y > window.getSize().y) {
				each.transform.pos.y = window.getSize().y - size_y;
				each.transform.vel.y *= -1;
			}

			if (each.Type() == EntityType::bullet) {
				bullets.push_back(&each);
			}
			else if (each.Type() == EntityType::enemy) {
				enemies.push_back(&each);
			}
		}

		for (auto* b : bullets) {
			for (auto* e : enemies) {


				//if (b->colision.bb.intersects(e->colision.bb)){
				//	b->deactivate();
				//	e->deactivate();
				//}

			}
		}


	}


};
