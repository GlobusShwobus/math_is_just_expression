#pragma once

#include "vec2.h"
#include <SFML/Graphics.hpp>

class CTransform {
public:
	vec2 pos{ 0,0 };
	vec2 vel{ 0,0 };
	float angle = 0;

	CTransform(const vec2& POS, const vec2& VEL, float a) :pos(POS), vel(VEL), angle(a) {}
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

	Player(size_t ID, EntityType type) :Entity(ID, type), score(0) {}

};

//manager will read the JSON config, remember configs and then be responsible for creating entities
//spawning happens here and only here, instead of how it is in the course thus AddEntity will be void and just take param which to create
class EntityManager {

	std::unique_ptr<std::vector<Entity>> to_add;
	std::unique_ptr<std::vector<Entity>> entities;

	size_t total_entities;

	void RemoveInactive() {
		entities->erase(std::remove_if(entities->begin(), entities->end(), [&](Entity& ent) {
			return !ent.IsActive();
			}), entities->end());
	}

public:

	EntityManager(const size_t reserve_amount) {
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