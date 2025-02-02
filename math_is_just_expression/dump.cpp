//#include "Entity.h"


/*
bool Collision::DoesCollide(const Entity& en1, const Entity& en2) {
	bool collisionX = en1.position.x + en1.shape.getSize().x >= en2.position.x && en1.position.x <= en2.position.x + en2.shape.getSize().x;
	bool collisionY = en1.position.y + en1.shape.getSize().y >= en2.position.y && en1.position.y <= en2.position.y + en2.shape.getSize().y;

	return collisionX && collisionY;
}

vec2 Collision::GetReflectionDot(const Entity& collider, const Entity& object) {
	//for better physics also need to the object to react

	float overlap_left = collider.position.x + collider.shape.getSize().x - object.position.x;
	float overlap_right = object.position.x + object.shape.getSize().x - collider.position.x;

	float overlap_top = collider.position.y + collider.shape.getSize().y - object.position.y;
	float overlap_bottom = object.position.y + object.shape.getSize().y - collider.position.y;

	float minOverlap = std::min({ overlap_left ,overlap_right ,overlap_top ,overlap_bottom });
	const float offset = 0.01f; // Adjust as needed

	if (minOverlap == overlap_left) {
		return { -1,0 };
	}
	else if (minOverlap == overlap_right) {
		return { 1,0 };
	}
	else if (minOverlap == overlap_top) {
		return { 0,-1 };
	}
	else if (minOverlap == overlap_bottom) {
		return { 0,1 };
	}

	return { 0,0 };
}
*/