#pragma once

#include <cmath>
#include <algorithm>


#define PI 3.14159265

class vec2 {
public:

	float x = 0;
	float y = 0;

	vec2(const float X, const float Y) :x(X), y(Y) {}
	//equate
	bool operator==(const vec2& other)const {
		return(x == other.x) && (y == other.y);
	}
	bool operator!=(const vec2& other)const {
		return(x != other.x) && (y != other.y);
	}
	//addition
	vec2 operator+(const vec2& rhs)const {
		return vec2(x + rhs.x, y + rhs.y);
	}
	vec2& operator+=(const vec2& rhs) {
		x += rhs.x;
		y += rhs.y;
		return *this;
	}
	//subtraction
	vec2 operator-(const vec2& rhs)const {
		return vec2(x - rhs.x, y - rhs.y);
	}
	vec2 operator-=(const vec2& rhs) {
		x -= rhs.x;
		y -= rhs.y;
		return *this;
	}
	//multiplication
	vec2 operator*(const float scale)const {
		return vec2(x * scale, y * scale);
	}
	vec2& operator*=(const float scale) {
		x*= scale;
		y*= scale;
		return *this;
	}
	//division
	vec2 operator/(const float scale)const {
		return vec2(x / scale, y / scale);
	}
	vec2& operator/=(const float scale) {
		x /= scale;
		y /= scale;
		return *this;
	}

	float Dist(const vec2& end)const;
	float Lenght()const;
	vec2 Normalized()const;
};

namespace line {
	
	struct line {
		vec2 beg = { 0,0 };
		vec2 end = { 0,0 };

		line(const vec2& s, const vec2& e) :beg(s), end(e) {}
	};

	float Slope(const vec2& first, const vec2& second);
	float Angle(const line& first, const line& second);
	vec2 Intersection(const line& line1, const line& line2);
	float Sqrt(const float num);
}


enum class CollisionSide {
	left, top, right, bottom, no_overlap
};

class BoundingBox {
public:
	float x = 0.f;
	float y = 0.f;
	float width = 0.f;
	float height = 0.f;
	vec2 velocity = { 0,0 };

	BoundingBox(const vec2& pos, const vec2& vel, const vec2& dimensions) :x(pos.x), y(pos.y), width(dimensions.x), height(dimensions.y), velocity(vel) {}
	BoundingBox(float X, float Y, float WIDTH, float HEIGHT, const vec2& VEL) :x(X), y(Y), width(WIDTH), height(HEIGHT), velocity(VEL) {}
	BoundingBox() = default;

	BoundingBox& GetBoundingBox();
	bool Intersects(const BoundingBox& another)const;
	CollisionSide GetCollisionSide(const BoundingBox& another)const;
};

namespace Collision {

	vec2 ReflectVelocity(const BoundingBox& reflected, const BoundingBox& from);
	vec2 BlockFurtherMove(const BoundingBox& guilty, const BoundingBox& another);

}

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