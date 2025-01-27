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
	float radius = 0;
	CCollision(float rad) :radius(rad) {}
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
	player, enemy, NULLTYPE
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

	Player(const nlohmann::json& config) :Entity(-1, EntityType::player), score(0) {
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

	}

};

/*
BIG JOB

make private functions to spawn each type of shit

decide what to do with config, either just keep it in json OR create custom structs for each, think JSON is more flexible but might be slower

then unite that somehow in AddEntity (acts as a factory basically)

*/
class EntityManager {

	std::unique_ptr<std::vector<Entity>> to_add;
	std::unique_ptr<std::vector<Entity>> entities;

	size_t total_entities = 0;

	void RemoveInactive() {
		entities->erase(std::remove_if(entities->begin(), entities->end(), [&](Entity& ent) {
			return !ent.IsActive();
			}), entities->end());
	}

public:

	EntityManager(const nlohmann::json& config, const size_t reserve_amount = 100) {
		to_add = std::make_unique<std::vector<Entity>>();
		entities = std::make_unique<std::vector<Entity>>();
		entities->reserve(reserve_amount);
		to_add->reserve(reserve_amount);
	}

	void Update() {
		//avoids iterator invalidation?
		std::move(to_add->begin(), to_add->end(), std::back_inserter(*entities));
		to_add->clear();
		RemoveInactive();
	}
	void AddEntity(const EntityType type, const vec2& pos) {
		//aint shit gonna happen untill we have JSON parsing done
	}


	std::vector<Entity>& GetEntities() {
		return *entities;
	}

};