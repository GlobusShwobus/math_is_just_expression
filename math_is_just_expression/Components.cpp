#include "Components.h"


float vec2::Dist(const vec2& end)const {
	const float A = (end.x - x) * 2;
	const float B = (end.y - y) * 2;

	return std::sqrtf(A + B);
}

float vec2::Lenght()const {
	return std::sqrtf((x * x) + (y * y));
}

vec2 vec2::Normalized()const {
	float len = Lenght();
	if (len == 0)return vec2(0, 0);
	return { x / len, y / len };
}

////////////////////////////////////////////////////B O U N D I N G  B O X//////////////////////////////////////////
////////////////////////////////////////////////////B O U N D I N G  B O X//////////////////////////////////////////
////////////////////////////////////////////////////B O U N D I N G  B O X//////////////////////////////////////////
////////////////////////////////////////////////////B O U N D I N G  B O X//////////////////////////////////////////
////////////////////////////////////////////////////B O U N D I N G  B O X//////////////////////////////////////////
BoundingBox& BoundingBox::GetBoundingBox() {
	return *this;
}

bool BoundingBox::Intersects(const BoundingBox& another)const {

	bool colX = x + width >= another.x && x <= another.x + another.width;
	bool colY = y + height >= another.y && y <= another.y + another.height;

	return colX && colY;
}

CollisionSide  BoundingBox::GetCollisionSide(const BoundingBox& another)const {

	float overlap_left = x + width - another.x;
	float overlap_right = another.x + another.width - x;
	float overlap_top = y + height - another.y;
	float overlap_bottom = another.y + another.height - y;


	float minOverLap = std::min({ overlap_left ,overlap_right ,overlap_top ,overlap_bottom });

	if (minOverLap == overlap_left) { return CollisionSide::left; }
	else if (minOverLap == overlap_right) { return CollisionSide::right; }
	else if (minOverLap == overlap_top) { return CollisionSide::top; }
	else if (minOverLap == overlap_bottom) { return CollisionSide::bottom; }
	else
		return CollisionSide::no_overlap;
}

vec2 Collision::ReflectVelocity(const BoundingBox& reflected, const BoundingBox& from) {

	CollisionSide colSide = reflected.GetCollisionSide(from);

	vec2 norm = { 0,0 };

	switch (colSide) {
	case CollisionSide::left:     norm.x = -1; break;
	case CollisionSide::right:    norm.x = 1;  break;
	case CollisionSide::top:      norm.y = -1; break;
	case CollisionSide::bottom:   norm.y = 1;  break;
	default:                                   return { reflected.velocity };
	}

	float dot = reflected.velocity.x * norm.x + reflected.velocity.y * norm.y;

	return{ reflected.velocity.x - (2 * dot * norm.x),reflected.velocity.y - (2 * dot * norm.y) };
}
vec2 Collision::BlockFurtherMove(const BoundingBox& guilty, const BoundingBox& another) {

	CollisionSide colSide = guilty.GetCollisionSide(another);

	vec2 block = { guilty.x, guilty.y };

	switch (colSide) {
	case CollisionSide::left:
		block.x = another.x - guilty.width;
		break;
	case CollisionSide::right:
		block.x = another.x + another.width;
		break;
	case CollisionSide::top:
		block.y = another.y - guilty.height;
		break;
	case CollisionSide::bottom:
		block.y = another.y + another.height;
		break;
	default:
		break;
	}
	return block;
}



 ////////////////////////////////////////////////////C U R E N T L Y    M I S C//////////////////////////////////////
 ////////////////////////////////////////////////////C U R E N T L Y    M I S C//////////////////////////////////////
 ////////////////////////////////////////////////////C U R E N T L Y    M I S C//////////////////////////////////////
 ////////////////////////////////////////////////////C U R E N T L Y    M I S C//////////////////////////////////////
 ////////////////////////////////////////////////////C U R E N T L Y    M I S C//////////////////////////////////////


float line::Slope(const vec2& first, const vec2& second) {
	float ex = second.x - first.x;
	float ey = second.y - first.y;

	if (ex == 0) {
		return 0.f;
	}

	return ey / ex;
}
float line::Angle(const line& first, const line& second) {
	float this_slope = Slope(first.beg, first.end);
	float other_slope = Slope(second.beg, second.end);

	float angle = std::abs((other_slope - this_slope) / (1 + this_slope * other_slope));

	float ret = atan(angle);

	return (ret * 180) / PI;
}

vec2 line::Intersection(const line& line1, const line& line2) {

	static constexpr float epsilon = 1e-6f;

	float a1 = line1.end.y - line1.beg.y;
	float b1 = line1.beg.x - line1.end.x;
	float c1 = a1 * (line1.beg.x) + b1 * (line1.beg.y);

	float a2 = line2.end.y - line2.beg.y;
	float b2 = line2.beg.x - line2.end.x;
	float c2 = a2 * (line2.beg.x) + b2 * (line2.beg.y);

	float determinant = a1 * b2 - a2 * b1;

	if (fabs(determinant) < epsilon) {
		return { FLT_MAX, FLT_MAX };
	}

	float x = (b2 * c1 - b1 * c2) / determinant;
	float y = (a1 * c2 - a2 * c1) / determinant;

	return { x,y };
}

float line::Sqrt(const float num) {//idk, apperantly standart lib one uses hardware to optimize or some shit
	static const float epsilon = 1e-6;
	float temp = 0;
	float sr = num / 2;

	do {
		temp = sr;
		sr = (temp + (num / temp)) / 2;
	} while (std::abs(temp - sr) > epsilon);

	return sr;
}