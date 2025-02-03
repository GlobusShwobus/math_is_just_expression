#pragma once

#include "Components.h"
#include <SFML/Graphics.hpp>

#include "json.hpp"

enum class EntityType {
	player, enemy, bullet, obstacle, stickybomb, NULLTYPE
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
	void SetPosition(const vec2& pos) {
		transform.x = pos.x;
		transform.y = pos.y;
	}
};


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

struct BasicConf {
	float size_x = 0;
	float size_y = 0;
	sf::Color fill;
	sf::Color out;
	int outline_thickness = 0;
	float speed = 0.f;
};

class EntityManager {

	std::vector<std::shared_ptr<Entity>> add_next_frame;
	std::vector<std::shared_ptr<Entity>> all;
	std::map<EntityType, std::vector<std::shared_ptr<Entity>>> per_type;
	
	size_t total_entities = 0;

	BasicConf enemyconf;
	BasicConf bulletconf;
	BasicConf obstacleconf;
	BasicConf stickyconf;


	vec2 window_size = { 0,0 };

	void InitLocalConfig(const nlohmann::json& config);

	bool PosIsFree(vec2& pos)const;

	void RemoveInactive();

public:

	EntityManager(const nlohmann::json& CONFIG) {
		InitLocalConfig(CONFIG);
	}

	void Update();

	void AddEnemy(vec2& POS, const vec2& VEL);
	void AddBullet(const vec2& origin, const vec2& target);
	void AddStickyBomb(const vec2& origin, const vec2& target);
	void AddObstacle(vec2& POS);

	const std::vector<std::shared_ptr<Entity>>& GetEntities()const {
		return all;
	}
	const std::vector<std::shared_ptr<Entity>>& GetEntities(const EntityType t) {
		return per_type[t];
	}
	const std::map<EntityType, std::vector<std::shared_ptr<Entity>>>& GetEntitiesMap()const {
		return per_type;
	}
};