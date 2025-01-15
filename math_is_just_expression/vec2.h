#pragma once
#include <cmath>

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
	float LenghtSquared()const {
		return (x * x) + (y * y);
	}
	float Lenght()const {
		return std::sqrtf((x * x) + (y * y));
	}

	vec2 Normalized()const {
		float len = Lenght();
		if (len == 0)return vec2(0, 0);
		return { x / len, y / len };
	}

};


class line {
	vec2 start = { 0,0 };
	vec2 end = { 0,0 };
public:
	line(const vec2& s, const vec2& e) :start(s), end(e) {}

	static float Sqrt(const float num) {//idk, apperantly standart lib one uses hardware to optimize or some shit
		static const float epsilon = 1e-6;
		float temp = 0;
		float sr = num / 2;

		do {
			temp = sr;
			sr = (temp + (num / temp)) / 2;
		} while (std::abs(temp - sr) > epsilon);

		return sr;
	}

	float Slope()const {
		float ex = end.x - start.x;
		float ey = end.y - start.y;
		return ey / ex;
	}
	float Angle(const line& other) {
		float this_slope = Slope();
		float other_slope = other.Slope();

		float angle = std::abs((other_slope - this_slope) / (1 + this_slope * other_slope));


	}

	bool Intersect();

};