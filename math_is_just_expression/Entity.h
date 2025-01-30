#pragma once

#include "vec2.h"
#include <SFML/Graphics.hpp>

#include "json.hpp"

class CScore {
public:
	int score = 0;
	CScore(int s) :score(s){}
};

class CLifespan {
public:
	int remaining = 0;
	int total = 0;
	CLifespan(int initial) :remaining(initial), total(initial) {}
	CLifespan() = default;
};

class CInput {
public:
	bool up = false;
	bool left = false;
	bool right = false;
	bool down = false;
	bool shield = false;
	bool shoot = false;

	CInput() = default;
};

class EntityManager;//pre declare or whatever the fuck

enum class EntityType {
	player, enemy, bullet, obstacle, NULLTYPE
};

class Entity {

	bool activeStatus = true;
	size_t id = 0;
	EntityType type = EntityType::NULLTYPE;

public:

	Entity(const size_t ID, const EntityType t):id(ID), type(t) {}


	vec2 position{ 0,0 };
	vec2 velocity{ 0,0 };
	sf::RectangleShape shape;
	CLifespan     lifepoints;


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
};

//redefine score later to be read from JSON, basically remember the score, so don't use the  0 basically

class Player: public Entity {

public:

	CScore score;
	CInput input;
	unsigned int speed = 0;

	Player(const nlohmann::json& config, const vec2& pos) :Entity(-1, EntityType::player), score(0) {
		if (!(config.contains("Entities") && config["Entities"].contains("Player"))) {
			throw std::runtime_error("Reading from JSON config file error");
		}

		//will throw if something is fucked up

		auto& sh = config["Entities"]["Player"];

		this->shape.setSize({ sh["size_x"], sh["size_y"] });//maybe error because ints
		this->shape.setFillColor({ sh["Fill_color"][0],sh["Fill_color"][1] ,sh["Fill_color"][2] });//no, fuck you.
		this->shape.setOutlineColor({ sh["Outline_color"][0],sh["Outline_color"][1] ,sh["Outline_color"][2] });
		this->shape.setOutlineThickness(sh["Outline_thickness"]);
		this->speed = sh["Base_speed"];
		this->position = pos;

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


		}
		catch (const std::exception&e) {
			printf("\n%s", e.what());
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

	void AddEnemy(const vec2& POS, const vec2& VEL) {

		auto rarefrog = std::shared_ptr<Entity>(new Entity(total_entities++, EntityType::enemy));

		rarefrog->shape.setSize({ enemyconf.size_x,enemyconf.size_y });
		rarefrog->shape.setFillColor(enemyconf.fill);
		rarefrog->shape.setOutlineColor(enemyconf.out);
		rarefrog->shape.setOutlineThickness(enemyconf.outline_thickness);

		rarefrog->velocity = VEL;
		rarefrog->position = POS;


		add_next_frame.push_back(rarefrog);
	}
	void AddBullet(const vec2& origin, const sf::Vector2f& target) {

		auto rarefrog = std::shared_ptr<Entity>(new Entity(total_entities++, EntityType::bullet));


		rarefrog->shape.setSize({ bulletconf.size_x,bulletconf.size_y });
		rarefrog->shape.setFillColor(bulletconf.fill);
		rarefrog->shape.setOutlineColor(bulletconf.out);
		rarefrog->shape.setOutlineThickness(bulletconf.outline_thickness);

		vec2 direction = { target.x - origin.x, target.y - origin.y };

		float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
		direction /= length;

		rarefrog->velocity = direction * bulletconf.speed;
		rarefrog->position = origin;


		add_next_frame.push_back(rarefrog);
	}
	void AddObstacle(const vec2& POS) {
		auto rarefrog = std::shared_ptr<Entity>(new Entity(total_entities++, EntityType::obstacle));

		rarefrog->shape.setSize({ obstacleconf.size_x,obstacleconf.size_y });
		rarefrog->shape.setFillColor(obstacleconf.fill);
		rarefrog->shape.setOutlineColor(obstacleconf.out);
		rarefrog->shape.setOutlineThickness(obstacleconf.outline_thickness);

		rarefrog->velocity = { obstacleconf.speed , obstacleconf.speed };
		rarefrog->position = POS;

		add_next_frame.push_back(rarefrog);
	}

	const std::vector<std::shared_ptr<Entity>>& GetEntities()const {
		return all;
	}
	const std::vector<std::shared_ptr<Entity>>& GetEntities(const EntityType t) {
		return per_type[t];
	}
};