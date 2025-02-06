#include "Entity.h"

/*
void EntityManager::InitLocalConfig(const nlohmann::json& config) {
	try {
		auto& sh = config["Entities"];

		//basic enemy
		enemyconf.size_x = sh["Enemy"]["size_x"];
		enemyconf.size_y = sh["Enemy"]["size_y"];
		enemyconf.fill = sf::Color{ sh["Enemy"]["Fill_color"][0],sh["Enemy"]["Fill_color"][1] ,sh["Enemy"]["Fill_color"][2] };
		enemyconf.out = sf::Color{ sh["Enemy"]["Outline_color"][0],sh["Enemy"]["Outline_color"][1] ,sh["Enemy"]["Outline_color"][2] };
		enemyconf.outline_thickness = sh["Enemy"]["Outline_thickness"];
		enemyconf.speed = sh["Enemy"]["Base_speed"];

		//basic bullet
		bulletconf.size_x = sh["Bullet"]["size_x"];
		bulletconf.size_y = sh["Bullet"]["size_y"];
		bulletconf.fill = sf::Color{ sh["Bullet"]["Fill_color"][0],sh["Bullet"]["Fill_color"][1] ,sh["Bullet"]["Fill_color"][2] };
		bulletconf.out = sf::Color{ sh["Bullet"]["Outline_color"][0],sh["Bullet"]["Outline_color"][1] ,sh["Bullet"]["Outline_color"][2] };
		bulletconf.outline_thickness = sh["Bullet"]["Outline_thickness"];
		bulletconf.speed = sh["Bullet"]["Base_speed"];

		//obstacle
		obstacleconf.size_x = sh["Obstacle"]["size_x"];
		obstacleconf.size_y = sh["Obstacle"]["size_y"];
		obstacleconf.fill = sf::Color{ sh["Obstacle"]["Fill_color"][0],sh["Obstacle"]["Fill_color"][1] ,sh["Obstacle"]["Fill_color"][2] };
		obstacleconf.out = sf::Color{ sh["Obstacle"]["Outline_color"][0],sh["Obstacle"]["Outline_color"][1] ,sh["Obstacle"]["Outline_color"][2] };
		obstacleconf.outline_thickness = sh["Obstacle"]["Outline_thickness"];
		obstacleconf.speed = sh["Obstacle"]["Base_speed"];

		//sticky
		stickyconf.size_x = sh["StickyBomb"]["size_x"];
		stickyconf.size_y = sh["StickyBomb"]["size_y"];
		stickyconf.fill = sf::Color{ sh["StickyBomb"]["Fill_color"][0],sh["StickyBomb"]["Fill_color"][1] ,sh["StickyBomb"]["Fill_color"][2] };
		stickyconf.out = sf::Color{ sh["StickyBomb"]["Outline_color"][0],sh["StickyBomb"]["Outline_color"][1] ,sh["StickyBomb"]["Outline_color"][2] };
		stickyconf.outline_thickness = sh["StickyBomb"]["Outline_thickness"];
		stickyconf.speed = sh["StickyBomb"]["Base_speed"];

		window_size = { config["Window"]["width"], config["Window"]["height"] };
	}
	catch (const std::exception& e) {
		printf("\n%s", e.what());
	}
}
void EntityManager::RemoveInactive() {

	all.erase(std::remove_if(all.begin(), all.end(), [&](std::shared_ptr<Entity>& ent) {
		return !ent->IsActive() || ent == nullptr;
		}), all.end());


	for (auto& [key, val] : per_type) {
		val.erase(std::remove_if(val.begin(), val.end(), [&](std::shared_ptr<Entity>& ent) {
			return !ent->IsActive() || ent == nullptr;
			}), val.end());
	}
}
void EntityManager::Update() {

	for (std::shared_ptr<Entity>& ent : add_next_frame) {
		all.push_back(ent);
		per_type[ent->Type()].push_back(ent);
	}

	add_next_frame.clear();
	RemoveInactive();
	printf("ent size: %d\n", (int)all.size());
}

void EntityManager::AddEnemy(vec2& POS, const vec2& VEL) {

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
void EntityManager::AddBullet(const vec2& origin, const vec2& target) {

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
void EntityManager::AddStickyBomb(const vec2& origin, const vec2& target) {

	auto rarefrog = std::shared_ptr<Entity>(new Entity(total_entities++, EntityType::stickybomb));


	rarefrog->shape.setSize({ stickyconf.size_x,stickyconf.size_y });
	rarefrog->shape.setFillColor(stickyconf.fill);
	rarefrog->shape.setOutlineColor(stickyconf.out);
	rarefrog->shape.setOutlineThickness(stickyconf.outline_thickness);

	vec2 direction = { target.x - origin.x, target.y - origin.y };

	float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
	direction /= length;


	direction *= stickyconf.speed;

	//the size is probably wrong because of outline thickness
	rarefrog->transform = { origin ,direction ,{ stickyconf.size_x , stickyconf.size_y } };

	add_next_frame.push_back(rarefrog);
}
void EntityManager::AddObstacle(vec2& POS) {
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


bool EntityManager::PosIsFree(vec2& pos)const {

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
*/