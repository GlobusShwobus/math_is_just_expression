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
