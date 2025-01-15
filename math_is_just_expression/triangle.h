#pragma once

#include "vec2.h"



//this is garbo, need a line class first

class triangle {
	vec2 A = { 0,0 };
	vec2 B = { 0,0 };
	vec2 C = { 0,0 };

	float len_A = 0.f;
	float len_B = 0.f;
	float len_C = 0.f;

	float angle_A = 0.f;
	float angle_B = 0.f;
	float angle_C = 0.f;
public:

	triangle(vec2 a, vec2 b, vec2 c) :A(a),B(b), C(c) {
		len_A = B.Dist(C);
		len_B = A.Dist(C);
		len_C = A.Dist(A);

	}

};