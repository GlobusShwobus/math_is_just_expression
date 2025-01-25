#pragma once

#include "vec2.h"
#include <SFML/Graphics.hpp>

class CTransform {
public:
	vec2 pos{ 0,0 };
	vec2 vel{ 0,0 };
	float angle = 0;

	CTransform(const vec2& POS, const vec2& VEL, float a) :pos(POS), vel(VEL), angle(a) {}
};

class CShape {
public:
	sf::RectangleShape rect;
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
};

class CInput {
public:
	bool up = false;
	bool left = false;
	bool right = false;
	bool down = false;
	bool shoot = false;

	CInput() = default;
};

class EntityManager;//pre declare or whatever the fuck

enum class EntityType {
	player, enemy, NULLTYPE
};

//break up entity into 3 types; NO POLYMORPHISM
//enviourment basic type
//bullet types(spells, bombs, skills whatever teh fuck)
//NPC  types
class Entity {

	bool activeStatus = false;
	size_t id = 0;
	EntityType type = EntityType::NULLTYPE;

public:

	Entity(const size_t id, const EntityType t);


	CTransform    transform;
	CShape        shape;
	CCollision    colision;
	CScore        score;
	CLifespan     lifepoints;
	CInput        input;

	bool IsActive()const;
	const EntityType Type()const;
	const size_t ID()const;
	void destroy();
	void deactivate();

};

class EntityManager {

	std::unique_ptr<std::vector<Entity>> to_add;
	std::unique_ptr<std::map<EntityType, std::vector<Entity>>> entities;


	size_t total_entities;

};