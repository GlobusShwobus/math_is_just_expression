#pragma once

#include "vec2.h"
#include <SFML/Graphics.hpp>

#include "json.hpp"

/*
class CScore {
public:
	int score = 0;
	CScore(int s) :score(s){}
};
*/


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

class CCollision {

	const sf::RectangleShape& from_shape;

public:

	bool DoesCollide(const sf::FloatRect& other)const {
	
		const sf::FloatRect bb = from_shape.getGlobalBounds();

		bool collisionX = bb.left + bb.width >= other.left && bb.left <= other.left + other.width;
		bool collisionY = bb.top + bb.height >= other.top && bb.top <= other.top + other.height;

		return collisionX && collisionY;
	}

	void SetReflectionVelocity(vec2& velocity, const sf::FloatRect& other_bb)const {

		vec2 normal(0.f, 0.f);
		const sf::FloatRect bb = from_shape.getGlobalBounds();

		float overlap_left = (bb.left + bb.width) - other_bb.left;
		float overlap_right = (other_bb.left + other_bb.width) - bb.left;

		float overlap_top = (bb.top + bb.height) - other_bb.top;
		float overlap_bottom = (other_bb.top + other_bb.height) - bb.top;


		float minOverlap = std::min({ overlap_left ,overlap_right ,overlap_top ,overlap_bottom });

		if (minOverlap == overlap_left)        { normal.x = -1; }
		else if (minOverlap == overlap_right)  { normal.x = 1; }
		else if (minOverlap == overlap_top)    { normal.y = -1; }
		else if (minOverlap == overlap_bottom) { normal.y = 1; }


		float dot = velocity.x * normal.x + velocity.y * normal.y;

		velocity.x -= (2 * dot * normal.x);
		velocity.y -= (2 * dot * normal.y);
	}

	
	CCollision(const sf::RectangleShape& for_bb) :from_shape(for_bb) {}
};


class EntityManager;//pre declare or whatever the fuck

enum class EntityType {
	player, enemy, bullet, obstacle, NULLTYPE
};


class Entity {

	friend class Player;
	friend class EntityManager;

	bool activeStatus = true;
	size_t id = 0;
	EntityType type = EntityType::NULLTYPE;


	Entity(const size_t ID, const EntityType t) :id(ID), type(t), collision(shape) {}

public:
	vec2 position{ 0,0 };
	vec2 velocity{ 0,0 };
	sf::RectangleShape shape;
	CLifespan     lifepoints;
	CCollision    collision;


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

	//CScore score;
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
		rarefrog->velocity = VEL;
		rarefrog->position = POS;

		rarefrog->shape.setSize({ enemyconf.size_x,enemyconf.size_y });
		rarefrog->shape.setFillColor(enemyconf.fill);
		rarefrog->shape.setOutlineColor(enemyconf.out);
		rarefrog->shape.setOutlineThickness(enemyconf.outline_thickness);


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
	void AddObstacle(vec2& POS) {
		auto rarefrog = std::shared_ptr<Entity>(new Entity(total_entities++, EntityType::obstacle));

		if (!PosIsFree(POS)) {
			printf("\nEntity outside window bounds");
			return;
		}

		rarefrog->velocity = { obstacleconf.speed , obstacleconf.speed };
		rarefrog->position = POS;

		rarefrog->shape.setSize({ obstacleconf.size_x,obstacleconf.size_y });
		rarefrog->shape.setFillColor(obstacleconf.fill);
		rarefrog->shape.setOutlineColor(obstacleconf.out);
		rarefrog->shape.setOutlineThickness(obstacleconf.outline_thickness);

		add_next_frame.push_back(rarefrog);
	}

	const std::vector<std::shared_ptr<Entity>>& GetEntities()const {
		return all;
	}
	const std::vector<std::shared_ptr<Entity>>& GetEntities(const EntityType t) {
		return per_type[t];
	}
};