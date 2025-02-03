#pragma once

#include "Components.h"
#include <SFML/Graphics.hpp>

#include "json.hpp"

enum class EntityType {
	player, enemy, bullet, obstacle, NULLTYPE
};


class EntityManager;//pre declare or whatever the fuck

class Entity {

	friend class Player;
	friend class EntityManager;
	Entity(const size_t ID, const EntityType t) :id(ID), type(t) {}


private:

	bool activeStatus = true;
	size_t id = 0;
	EntityType type = EntityType::NULLTYPE;

	sf::RectangleShape shape;
	BoundingBox transform;

public:

	bool IsActive()const {
		return activeStatus;
	}
	const EntityType Type()const {
		return type;
	}
	const size_t ID()const {
		return id;
	}
	void deactivate() {
		activeStatus = false;
	}

	const vec2 GetPosition()const {
		return { transform.x, transform.y };
	}
	BoundingBox& GetBoundingBox() {
		return transform.GetBoundingBox();
	}
	const sf::RectangleShape& GetShape()const {
		return shape;
	}
	void UpdatePosition() {
		transform.x += transform.velocity.x;
		transform.y += transform.velocity.y;
		shape.setPosition(transform.x, transform.y);
	}
	void SetVelocity(const vec2& vel) {
		transform.velocity = vel;
	}
};


//redefine score later to be read from JSON, basically remember the score, so don't use the  0 basically

class Player: public Entity {

public:

	CInput input;
	unsigned int speed = 0;

	Player(const nlohmann::json& config, const vec2& pos) :Entity(-1, EntityType::player) {
		if (!(config.contains("Entities") && config["Entities"].contains("Player"))) {
			throw std::runtime_error("Reading from JSON config file error");
		}

		//will throw if something is fucked up

		auto& sh = config["Entities"]["Player"];

		this->shape.setSize({ sh["size_x"], sh["size_y"] });//maybe error because ints
		this->shape.setFillColor({ sh["Fill_color"][0],sh["Fill_color"][1] ,sh["Fill_color"][2] });//no, fuck you.
		this->shape.setOutlineColor({ sh["Outline_color"][0],sh["Outline_color"][1] ,sh["Outline_color"][2] });
		this->shape.setOutlineThickness(sh["Outline_thickness"]);
		this->shape.setPosition(pos.x, pos.y);

		this->speed = sh["Base_speed"];
	}

};

struct EnemyConf {
	float size_x = 0;
	float size_y = 0;
	sf::Color fill;
	sf::Color out;
	int outline_thickness = 0;
	float speed = 0.f;
	int lifepoints = 0;
};
struct BulletConf {
	float size_x = 0;
	float size_y = 0;
	sf::Color fill;
	sf::Color out;
	int outline_thickness = 0;
	float speed = 0.f;
	int lifepoints = 0;
};
struct ObstacleConf {
	float size_x = 0;
	float size_y = 0;
	sf::Color fill;
	sf::Color out;
	int outline_thickness = 0;
	float speed = 0.f;
	int lifepoints = 0;
};


class EntityManager {

	std::vector<std::shared_ptr<Entity>> add_next_frame;
	std::vector<std::shared_ptr<Entity>> all;
	std::map<EntityType, std::vector<std::shared_ptr<Entity>>> per_type;
	
	size_t total_entities = 0;

	EnemyConf enemyconf;
	BulletConf bulletconf;
	ObstacleConf obstacleconf;


	vec2 window_size = { 0,0 };

	void InitLocalConfig(const nlohmann::json& config) {
		try {
			auto& sh = config["Entities"];

			//basic enemy
			enemyconf.size_x = sh["Enemy"]["size_x"];
			enemyconf.size_y = sh["Enemy"]["size_y"];
			enemyconf.fill = sf::Color{ sh["Enemy"]["Fill_color"][0],sh["Enemy"]["Fill_color"][1] ,sh["Enemy"]["Fill_color"][2] };
			enemyconf.out = sf::Color{ sh["Enemy"]["Outline_color"][0],sh["Enemy"]["Outline_color"][1] ,sh["Enemy"]["Outline_color"][2] };
			enemyconf.outline_thickness = sh["Enemy"]["Outline_thickness"];
			enemyconf.speed = sh["Enemy"]["Base_speed"];
			enemyconf.lifepoints = sh["Enemy"]["Lifepoints"];

			//basic bullet
			bulletconf.size_x = sh["Bullet"]["size_x"];
			bulletconf.size_y = sh["Bullet"]["size_y"];
			bulletconf.fill = sf::Color{ sh["Bullet"]["Fill_color"][0],sh["Bullet"]["Fill_color"][1] ,sh["Bullet"]["Fill_color"][2] };
			bulletconf.out = sf::Color{ sh["Bullet"]["Outline_color"][0],sh["Bullet"]["Outline_color"][1] ,sh["Bullet"]["Outline_color"][2] };
			bulletconf.outline_thickness = sh["Bullet"]["Outline_thickness"];
			bulletconf.speed = sh["Bullet"]["Base_speed"];
			bulletconf.lifepoints = sh["Bullet"]["Lifepoints"];

			//obstacle
			obstacleconf.size_x = sh["Obstacle"]["size_x"];
			obstacleconf.size_y = sh["Obstacle"]["size_y"];
			obstacleconf.fill = sf::Color{ sh["Obstacle"]["Fill_color"][0],sh["Obstacle"]["Fill_color"][1] ,sh["Obstacle"]["Fill_color"][2] };
			obstacleconf.out = sf::Color{ sh["Obstacle"]["Outline_color"][0],sh["Obstacle"]["Outline_color"][1] ,sh["Obstacle"]["Outline_color"][2] };
			obstacleconf.outline_thickness = sh["Obstacle"]["Outline_thickness"];
			obstacleconf.speed = sh["Obstacle"]["Base_speed"];
			obstacleconf.lifepoints = sh["Obstacle"]["Lifepoints"];

			window_size = { config["Window"]["width"], config["Window"]["height"] };
		}
		catch (const std::exception&e) {
			printf("\n%s", e.what());
		}
	}

	bool PosIsFree(vec2& pos) {

		while (true) {
			bool isFree = true;

			for (const auto& each : all) {
				const sf::FloatRect& bounds = each->shape.getGlobalBounds();

				if (bounds.contains(pos.x, pos.y)) {

					isFree = false;

					pos.x = bounds.left + bounds.width;


					if (pos.x >= window_size.x) {
						pos.x = 0; 
						pos.y += bounds.height;
					}

					if (pos.y >= window_size.y) {
						return false;
					}
					break;
				}
			}

			if (isFree) {
				return true;
			}
		}
	}

	void RemoveInactive() {

		all.erase(std::remove_if(all.begin(), all.end(), [&](std::shared_ptr<Entity>& ent) {
			return !ent->IsActive();
			}), all.end());


		for (auto& pair : per_type) {
			auto& it = pair.second;
			it.erase(std::remove_if(it.begin(), it.end(), [&](std::shared_ptr<Entity>& ent) {
				return !ent->IsActive();
				}), it.end());
		}
	}

public:

	EntityManager(const nlohmann::json& CONFIG) {
		InitLocalConfig(CONFIG);
	}

	void Update() {
		//avoids iterator invalidation?


		for (std::shared_ptr<Entity>& ent : add_next_frame) {
			all.push_back(ent);
			per_type[ent->Type()].push_back(ent);
		}

		add_next_frame.clear();
		RemoveInactive();
		printf("ent size: %d\n", (int)all.size());
	}

	void AddEnemy(vec2& POS, const vec2& VEL) {

		auto rarefrog = std::shared_ptr<Entity>(new Entity(total_entities++, EntityType::enemy));

		if (!PosIsFree(POS)) {
			printf("\nEntity outside window bounds");
			return;
		}

		rarefrog->shape.setSize({ enemyconf.size_x,enemyconf.size_y });
		rarefrog->shape.setFillColor(enemyconf.fill);
		rarefrog->shape.setOutlineColor(enemyconf.out);
		rarefrog->shape.setOutlineThickness(enemyconf.outline_thickness);

		rarefrog->transform = { POS,VEL, {enemyconf.size_x,enemyconf.size_y} };

		add_next_frame.push_back(rarefrog);
	}
	void AddBullet(const vec2& origin, const vec2& target) {

		auto rarefrog = std::shared_ptr<Entity>(new Entity(total_entities++, EntityType::bullet));


		rarefrog->shape.setSize({ bulletconf.size_x,bulletconf.size_y });
		rarefrog->shape.setFillColor(bulletconf.fill);
		rarefrog->shape.setOutlineColor(bulletconf.out);
		rarefrog->shape.setOutlineThickness(bulletconf.outline_thickness);

		vec2 direction = { target.x - origin.x, target.y - origin.y };

		float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
		direction /= length;


		direction *= bulletconf.speed;

		rarefrog->transform = { origin ,direction ,{ bulletconf.size_x,bulletconf.size_y } };

		add_next_frame.push_back(rarefrog);
	}
	void AddObstacle(vec2& POS) {
		auto rarefrog = std::shared_ptr<Entity>(new Entity(total_entities++, EntityType::obstacle));

		if (!PosIsFree(POS)) {
			printf("\nEntity outside window bounds");
			return;
		}

		rarefrog->shape.setSize({ obstacleconf.size_x,obstacleconf.size_y });
		rarefrog->shape.setFillColor(obstacleconf.fill);
		rarefrog->shape.setOutlineColor(obstacleconf.out);
		rarefrog->shape.setOutlineThickness(obstacleconf.outline_thickness);


		rarefrog->transform = { POS,  {0,0},{ obstacleconf.size_x,obstacleconf.size_y } };

		add_next_frame.push_back(rarefrog);
	}

	const std::vector<std::shared_ptr<Entity>>& GetEntities()const {
		return all;
	}
	const std::vector<std::shared_ptr<Entity>>& GetEntities(const EntityType t) {
		return per_type[t];
	}
};