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


		float darw_y_pos = window.getSize().y - obstacle_size_idk_where_to_put_atm;
		float darw_x_pos = 0;

		//bottom
		while (darw_x_pos - obstacle_size_idk_where_to_put_atm < window.getSize().x) {
			vec2 compilermemes = { darw_x_pos , darw_y_pos };
			entities.AddObstacle(compilermemes);
			darw_x_pos += obstacle_size_idk_where_to_put_atm;
		}
		
		//top
		darw_y_pos = 0;
		darw_x_pos = 0;
		while (darw_x_pos - obstacle_size_idk_where_to_put_atm < window.getSize().x) {
			vec2 compilermemes = { darw_x_pos, darw_y_pos };
			entities.AddObstacle(compilermemes);
			darw_x_pos += obstacle_size_idk_where_to_put_atm;
		}

		//left
		darw_x_pos = 0;
		darw_y_pos = 0;
		while (darw_y_pos - obstacle_size_idk_where_to_put_atm < window.getSize().y) {
			vec2 compilermemes = { darw_x_pos, darw_y_pos };
			entities.AddObstacle(compilermemes);
			darw_y_pos += obstacle_size_idk_where_to_put_atm;
		}

		//right
		darw_x_pos = window.getSize().x - obstacle_size_idk_where_to_put_atm;
		darw_y_pos = 0;
		while (darw_y_pos - obstacle_size_idk_where_to_put_atm < window.getSize().y) {
			vec2 compilermemes = { darw_x_pos, darw_y_pos };
			entities.AddObstacle(compilermemes);
			darw_y_pos += obstacle_size_idk_where_to_put_atm;
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


		for (auto& each : entities.GetEntities()) {
			window.draw(each->GetShape());
		}

		window.draw(player.GetShape());
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
				
					hold_test = true;
					sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
					entities.AddBullet(player.GetPosition(), { mousePos.x, mousePos.y });

					//player.input.shoot = true; damn guide is shit, ima do it my way
					//also spawn bullet here
					//handle spawning bullet somewhere
				}
				if (sfevent.key.code == sf::Mouse::Right) {
					sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
					entities.AddStickyBomb(player.GetPosition(), { mousePos.x, mousePos.y });
					//player.input.shield = true;
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
			each->UpdatePosition();
		}

		vec2 player_vec = { 0,0 };

		if (player.input.up) {
			player_vec.y -= player.speed;
		}
		if (player.input.left) {
			player_vec.x -= player.speed;
		}
		if (player.input.down) {
			player_vec.y += player.speed;
		}
		if (player.input.right) {
			player_vec.x += player.speed;
		}
		player.SetVelocity(player_vec);
		player.UpdatePosition();
	}

	void sCollision() {

		for (auto& bullets : entities.GetEntities(EntityType::bullet)) {
			for (auto& enemies : entities.GetEntities(EntityType::enemy)) {
				
				const auto& bulletsBB = bullets->GetBoundingBox();
				const auto& enemiesBB = enemies->GetBoundingBox();

				if (bulletsBB.Intersects(enemiesBB)) {
					bullets->deactivate();
					enemies->deactivate();
				}
			}
		}
		//////////////////////////////////////////
		// STICKY BOMB CODE

		std::vector<int> toChange;

		for (auto& sticky : entities.GetEntities(EntityType::stickybomb)) {
			for (auto& stickable : entities.GetEntities()) {


				if (stickable->Type() != EntityType::obstacle && stickable->Type() != EntityType::stickyAttached) {
					continue;
				}

				auto& stickyBB = sticky->GetBoundingBox();
				auto& anotherStickyBB = stickable->GetBoundingBox();



				if (anotherStickyBB.Intersects(stickyBB)) {
					sticky->SetPosition(Collision::BlockFurtherMove(stickyBB, anotherStickyBB));
					sticky->SetVelocity({ 0,0 });


					toChange.push_back(sticky->ID());
					//entities.ResteatType(sticky->ID(), sticky->Type(), EntityType::stickyAttached); //iterator invalidation
				}
			}
		}

		for (int id : toChange) {
			entities.ResteatType(id, EntityType::stickybomb, EntityType::stickyAttached);
		}

		// STICKY BOMB CODE
	    //////////////////////////////////////////


		// NEED A CUSTOM MAP WITH INT ENUM BIT MANIPULATION BULLSHIT
		for (auto& each : entities.GetEntities()) {

			auto& eachBB = each->GetBoundingBox();

			for (auto& obstacle : entities.GetEntities(EntityType::obstacle)) {
				auto& obstacleBB = obstacle->GetBoundingBox();
				if (eachBB.Intersects(obstacleBB)) {
					eachBB.velocity = Collision::ReflectVelocity(eachBB, obstacleBB);
				}
			}


			for (auto& stickables : entities.GetEntities(EntityType::stickyAttached)) {
				auto& stickablesBB = stickables->GetBoundingBox();
				if (eachBB.Intersects(stickablesBB)) {
					eachBB.velocity = Collision::ReflectVelocity(eachBB, stickablesBB);
				}
			}

		}


	}
};
