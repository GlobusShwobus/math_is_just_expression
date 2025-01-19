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



namespace line {

	struct line {
		vec2 beg = { 0,0 };
		vec2 end = { 0,0 };

		line(const vec2& s, const vec2& e) :beg(s), end(e) {}
	};

	float Slope(const vec2& first, const vec2& second) {
		float ex = second.x - first.x;
		float ey = second.y - first.y;

		if (ex == 0) {
			return 0.f;
		}

		return ey / ex;
	}
	float Angle(const line& first, const line& second) {
		float this_slope = Slope(first.beg, first.end);
		float other_slope = Slope(second.beg, second.end);

		float angle = std::abs((other_slope - this_slope) / (1 + this_slope * other_slope));

		float ret = atan(angle);

		return (ret * 180) / PI;
	}

	vec2 Intersection(const line& line1, const line& line2) {

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
}
