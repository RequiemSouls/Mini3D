#ifndef _MINI3D_GEOMETRY_H_
#define _MINI3D_GEOMETRY_H_

#include "Types.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

struct Vec2 {
	float x = 0;
	float y = 0;

	Vec2() { }
	Vec2(float px, float py) : x(px), y(py) { }

	Vec2 operator - (const Vec2& other) {
		return Vec2(x - other.x, y - other.y);
	}

	Vec2 operator + (const Vec2& other) {
		return Vec2(x + other.x, y + other.y);
	}

	Vec2& operator += (const Vec2& other) {
		x += other.x;
		y += other.y;
		return *this;
	}

	Vec2& operator -= (const Vec2& other) {
		x -= other.x;
		y -= other.y;
		return *this;
	}

	float length() {
		return sqrt(x*x + y*y);
	}

	void normalized() {
		float len = sqrt(x*x + y*y);
		x /= len;
		y /= len;
	}

	static const Vec2 ZERO;
};

struct Vector {
    float x = 0.f;
	float y = 0.f;
	float z = 0.f;
	float w = 1.f;

    Vector() {
    }


	static const Vector ZERO;
};

struct Matrix {
	float m[4][4] = {0};

	Matrix() {}
	Matrix(float* mat) { memcpy(m, mat, 16); }

	static const Matrix ZERO;
	static const Matrix IDENTITY;

};

struct Color {
    UI8 r = 0;
	UI8 g = 0;
	UI8 b = 0;

	Color() {}
	Color(UI8 cr, UI8 cg, UI8 cb) { r = cr; g = cg; b = cb; }

	static const Color ZERO;
};

struct Vertex{
    Vector p;
    Color c;
};
#endif