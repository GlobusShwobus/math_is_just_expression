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


		window.create(sf::VideoMode(sh["width"].get<int>(), sh["height"].get<int>()), sh["game_name"].get<std::string>());
		window.setFramerateLimit(sh["framerate_cap"].get<int>());





		static constexpr int obstacle_size_idk_where_to_put_atm = 20;


		//draw bottom floor type thing
		//dont forget to del this 
		float darw_y_pos = window.getSize().y - obstacle_size_idk_where_to_put_atm;
		float darw_x_pos = 0;

		while (darw_x_pos - obstacle_size_idk_where_to_put_atm < window.getSize().x) {
			vec2 compilermemes = { darw_x_pos ,darw_y_pos };
			entities.AddObstacle(compilermemes);
			darw_x_pos += obstacle_size_idk_where_to_put_atm;
		}


		static constexpr int platform_size = 7;
		
		float y_platform1_start = 100;
		float x_platform1_start = 100;

		float y_platform2_start = 200;
		float x_platform2_start = 240;

		float y_platform3_start = 300;
		float x_platform3_start = 380;

		float y_platform4_start = 400;
		float x_platform4_start = 520;

		float y_platform5_start = 500;
		float x_platform5_start = 660;

		float y_platform6_start = 600;
		float x_platform6_start = 800;


		for (int i = 0; i < platform_size; ++i) {
			vec2 compilermemes = { x_platform1_start ,y_platform1_start };
			x_platform1_start += obstacle_size_idk_where_to_put_atm;
			entities.AddObstacle(compilermemes);
		}
		for (int i = 0; i < platform_size; ++i) {
			vec2 compilermemes = { x_platform2_start ,y_platform2_start };
			x_platform2_start += obstacle_size_idk_where_to_put_atm;
			entities.AddObstacle(compilermemes);
		}
		for (int i = 0; i < platform_size; ++i) {
			vec2 compilermemes = { x_platform3_start ,y_platform3_start };
			x_platform3_start += obstacle_size_idk_where_to_put_atm;
			entities.AddObstacle(compilermemes);
		}
		for (int i = 0; i < platform_size; ++i) {
			vec2 compilermemes = { x_platform4_start ,y_platform4_start };
			x_platform4_start += obstacle_size_idk_where_to_put_atm;
			entities.AddObstacle(compilermemes);
		}
		for (int i = 0; i < platform_size; ++i) {
			vec2 compilermemes = { x_platform5_start ,y_platform5_start };
			x_platform5_start += obstacle_size_idk_where_to_put_atm;
			entities.AddObstacle(compilermemes);
		}
		for (int i = 0; i < platform_size; ++i) {
			vec2 compilermemes = { x_platform6_start ,y_platform6_start };
			x_platform6_start += obstacle_size_idk_where_to_put_atm;
			entities.AddObstacle(compilermemes);
		}

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

		//temp garbo

		static size_t last_spawn = 0;
		static std::uniform_int_distribution<> pos(0, 999);
		static std::uniform_int_distribution<> vel(-5, 5);
		
		if ((current_frame - last_spawn) >= 90) {
			vec2 POS = { (float)pos(rng), (float)pos(rng) };
			entities.AddEnemy(POS, { (float)vel(rng), (float)vel(rng) });
			last_spawn = current_frame;
		}

	}

	void sRender() {

		window.clear();


		player.shape.setPosition(player.position.x, player.position.y);

		
		for (auto& each : entities.GetEntities()) {
			
			each->shape.setPosition(each->position.x, each->position.y);
			window.draw(each->shape);
		}

		window.draw(player.shape);
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
			static bool hold_test = false;
			if (sfevent.type == sf::Event::MouseButtonPressed || hold_test) {
				if (sfevent.key.code == sf::Mouse::Left || hold_test) {
					player.input.shoot = true;
					hold_test = true;
					sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

					entities.AddBullet({ player.position }, mousePos);
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
				hold_test = false;
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
			each->position += each->velocity;
		}

		player.velocity = { 0,0 };		//reset vel each frame

		if (player.input.up) {
			player.velocity.y -= player.speed;
		}
		if (player.input.left) {
			player.velocity.x -= player.speed;
		}
		if (player.input.down) {
			player.velocity.y += player.speed;
		}
		if (player.input.right) {
			player.velocity.x += player.speed;
		}



		player.position += player.velocity;
	}

	void sCollision() {

		for (auto& each : entities.GetEntities()) {

			float size_x = each->shape.getSize().x;
			float size_y = each->shape.getSize().y;

			//for left and right edges
			if (each->position.x < 0) {
				each->position.x = 0;
				each->velocity.x *= -1;
			}
			else if (each->position.x + size_x > window.getSize().x) {
				each->position.x = window.getSize().x - size_x;
				each->velocity.x *= -1;
			}
			//for up and down edges
			if (each->position.y < 0) {
				each->position.y = 0;
				each->velocity.y *= -1;
			}
			else if (each->position.y + size_y > window.getSize().y) {
				each->position.y = window.getSize().y - size_y;
				each->velocity.y *= -1;
			}

		}


		for (auto& bullets : entities.GetEntities(EntityType::bullet)) {
			for (auto& enemes : entities.GetEntities(EntityType::enemy)) {
				if (bullets->shape.getGlobalBounds().intersects(enemes->shape.getGlobalBounds())) {
					bullets->deactivate();
					enemes->deactivate();
				}
			}
		}

		for (auto& obstacle : entities.GetEntities(EntityType::obstacle)) {
			for (auto& each : entities.GetEntities()) {

				if (each->Type() == EntityType::obstacle) {
					continue;
				}
				if (Collision::DoesCollide(*each, *obstacle)) {
					vec2 dir = Collision::GetReflectionDot(*each, *obstacle);
					each->velocity.dot(dir);
				}

			}
		}


	}


};
