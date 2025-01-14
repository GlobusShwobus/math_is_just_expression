#pragma once
#include <cmath>

class vec2 {
public:

	float x = 0;
	float y = 0;

	vec2(const float X, const float Y) :x(X), y(Y) {}
	
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

	float Sqrt(const float num) {
		float temp = 0;
		float sr = num / 2;

		do {
			temp = sr;
			sr = (temp + (num / temp)) / 2;
		} while ((temp - sr) != 0);

		return sr;
	}
	float DistSquared(const vec2& end)const {
		const float A = (end.x - x) * 2;
		const float B = (end.y - y) * 2;

		return A + B;
	}
	float Dist(const vec2& end)const {
		const float A = (end.x - x) * 2;
		const float B = (end.y - y) * 2;
		
		return std::sqrtf(A + B);
	}

};