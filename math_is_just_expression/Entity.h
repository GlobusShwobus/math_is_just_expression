#pragma once

#include "vec2.h"
#include <SFML/Graphics.hpp>
#include "json.hpp"

class CTransform {
public:
	vec2 pos{ 0,0 };
	vec2 vel{ 0,0 };
	float speed = 0;

	CTransform(const vec2& POS, const vec2& VEL, float s) :pos(POS), vel(VEL), speed(s) {}
	CTransform() = default;
};

class CShape {
public:
	sf::RectangleShape rect;
	CShape() = default;
	CShape(const sf::Vector2f& size, const sf::Color& fill, const sf::Color& outline, float thickness)
		:rect(size) {
		rect.setFillColor(fill);
		rect.setOutlineColor(outline);
		rect.setOutlineThickness(thickness);
	}
};

class CCollision {
public:
	sf::FloatRect bb;
	CCollision(sf::FloatRect& BB) :bb(BB) {}
	CCollision() = default;
};

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
	player, enemy, bullet, NULLTYPE
};

class Entity {

	bool activeStatus = true;
	bool destroyStatus = false;
	size_t id = 0;
	EntityType type = EntityType::NULLTYPE;

public:

	Entity(const size_t ID, const EntityType t):id(ID), type(t) {}

	CTransform    transform;
	CShape        shape;
	CCollision    colision;
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
	void destroy() {
		destroyStatus = true;
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

	Player(const nlohmann::json& config, const vec2& pos) :Entity(-1, EntityType::player), score(0) {
		if (!(config.contains("Entities") && config["Entities"].contains("Player"))) {
			throw std::runtime_error("Reading from JSON config file error");
		}

		//will throw if something is fucked up

		auto& sh = config["Entities"]["Player"];

		this->shape.rect.setSize({ sh["size_x"], sh["size_y"] });//maybe error because ints
		this->shape.rect.setFillColor({ sh["Fill_color"][0],sh["Fill_color"][1] ,sh["Fill_color"][2] });//no, fuck you.
		this->shape.rect.setOutlineColor({ sh["Outline_color"][0],sh["Outline_color"][1] ,sh["Outline_color"][2] });
		this->shape.rect.setOutlineThickness(sh["Outline_thickness"]);
		this->transform.speed = sh["Base_speed"];
		this->transform.pos = pos;

		this->colision.bb = this->shape.rect.getLocalBounds();

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

class EntityManager {

	std::unique_ptr<std::vector<Entity>> to_add;
	std::unique_ptr<std::vector<Entity>> entities;
	size_t total_entities = 0;


	EnemyConf enemyconf;
	BulletConf bulletconf;

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


		}
		catch (const std::exception&e) {
			printf("\n%s", e.what());
		}
	}


	void RemoveInactive() {
		entities->erase(std::remove_if(entities->begin(), entities->end(), [&](Entity& ent) {
			return !ent.IsActive();
			}), entities->end());
	}


public:

	EntityManager(const nlohmann::json& CONFIG, const size_t reserve_amount = 100) {
		to_add = std::make_unique<std::vector<Entity>>();
		entities = std::make_unique<std::vector<Entity>>();
		entities->reserve(reserve_amount);
		to_add->reserve(reserve_amount);

		InitLocalConfig(CONFIG);
	}

	void Update() {
		//avoids iterator invalidation?
		std::move(to_add->begin(), to_add->end(), std::back_inserter(*entities));
		to_add->clear();
		RemoveInactive();
	}
	void AddEntity(const EntityType type, const vec2& _pos) {

		Entity rarefrog(total_entities++, type);

		if (type == EntityType::enemy) {
			rarefrog.shape.rect.setSize({ enemyconf.size_x,enemyconf.size_y });
			rarefrog.shape.rect.setFillColor(enemyconf.fill);
			rarefrog.shape.rect.setOutlineColor(enemyconf.out);
			rarefrog.shape.rect.setOutlineThickness(enemyconf.outline_thickness);
			rarefrog.transform.speed = enemyconf.speed;
			rarefrog.transform.pos = _pos;

			rarefrog.colision.bb = rarefrog.shape.rect.getLocalBounds();
		}
		else if (type == EntityType::bullet) {
			rarefrog.shape.rect.setSize({ bulletconf.size_x,bulletconf.size_y });
			rarefrog.shape.rect.setFillColor(bulletconf.fill);
			rarefrog.shape.rect.setOutlineColor(bulletconf.out);
			rarefrog.shape.rect.setOutlineThickness(bulletconf.outline_thickness);
			rarefrog.transform.speed = bulletconf.speed;
			rarefrog.transform.pos = _pos;

			rarefrog.colision.bb = rarefrog.shape.rect.getLocalBounds();
		}


		to_add->push_back(std::move(rarefrog));
	}


	std::vector<Entity>& GetEntities() {
		return *entities;
	}

};