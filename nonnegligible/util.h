#pragma once

#ifndef util_h
#define util_h

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <math.h>

#define PI 3.14159f

struct vec2D {
	vec2D() {}
	vec2D(float x, float y) { this->x = x; this->y = y; }
	float x = 0.0f;
	float y = 0.0f;
	bool operator == (vec2D rhs) const { return (this->x == rhs.x) && (this->x == rhs.y); }
	vec2D operator + (vec2D rhs) const { return vec2D(this->x + rhs.x, this->y + rhs.y);  }
	vec2D operator - (vec2D rhs) const { return vec2D(this->x - rhs.x, this->y - rhs.y); }
	vec2D operator * (float factor) const { return vec2D(this->x * factor, this->y * factor); }
	vec2D operator / (float factor) const { return vec2D(this->x / factor, this->y / factor); }
	vec2D& operator += (vec2D rhs) { this->x += rhs.x; this->y += rhs.y; return *this; }
	vec2D& operator -= (vec2D rhs) { this->x -= rhs.x; this->y -= rhs.y; return *this; }
	vec2D& operator *= (float factor) { this->x *= factor; this->y *= factor; return *this; }
	vec2D& operator /= (float factor) { this->x /= factor; this->y /= factor; return *this; }
	float mag() { return sqrt(this->x * this->x + this->y * this->y); }
	vec2D norm() { return { this->x / mag(), this->y / mag() }; }
	// vec2D abs() { return vec2D(std::abs(this->x), std::abs(this->y)); }
};

struct AABB {
	vec2D min;
	vec2D max;
};

struct line {
	vec2D start;
	vec2D stop;
};

// treating a struct as a vector here feels wrong, but compiler says its ok sooooooo
inline vec2D vec2DAdd(vec2D& a, vec2D& b) { return { a.x + b.x, a.y + b.y }; }
inline vec2D vec2DSub(vec2D& a, vec2D& b) { return { a.x - b.x, a.y - b.y }; }
inline vec2D vec2DMult(vec2D& a, const float& val) { return { val * a.x, val * a.y }; }
inline vec2D vec2DDiv(vec2D& a, const float& val) { return { a.x / val, a.y / val }; }
inline vec2D vec2DElementwiseMult(vec2D& a, vec2D& b) { return { a.x * b.x, a.y * b.y }; }
inline vec2D vec2DElementwiseDiv(vec2D& a, vec2D& b) { return { a.x / b.x, a.y / b.y }; }
inline vec2D dotProduct2D(vec2D& a, vec2D& b) { return { a.x * b.x, a.y * b.y }; }
inline float vec2DMag(vec2D& vec) { return sqrt((vec.x * vec.x) + vec.y * vec.y); }
inline vec2D vec2DNormalise(vec2D& vec) { return vec2DDiv(vec, vec2DMag(vec)); }
inline float vec2DDotProduct(vec2D& vec1, vec2D& vec2) { return (vec1.x * vec2.x) + (vec1.y + vec2.y); }
inline vec2D vec2DAbs(vec2D& vec) { return { std::abs(vec.x), std::abs(vec.y) }; }

// d = (v_i)(t) + 0.5(a)(t^2)
inline float getDisp1(float initVel, float accel, float time) { return (initVel * time) + (0.5 * accel * time * time); }
inline float getDisp2(float initVel, float finalVel, float time) { return (time * (initVel + finalVel) / 2); }
inline float getFinalVel1(float initVel, float accel, float time) { return initVel + (time * accel); }
inline float getFinalVel2(float initVel, float accel, float disp) { return sqrt((initVel * initVel) + (2 * accel * disp)); }
inline float lerp(float init, float end, float factor) { return init + (factor * (end - init)); }

// rads to deg and vice versa
inline float radToDeg(float& rad) { return rad * 180.0f / PI; }
inline float degToRad(float& deg) { return deg * PI / 180.0f; }

// distance
inline float getDist(vec2D& a, vec2D& b) { return sqrt(((b.y - a.y) * (b.y - a.y)) + ((b.x - a.x) * (b.x - a.x))); }

// powers
template <typename T>
T square(T num) { return num * num; }
template <typename T>
T cube(T num) { return num * num * num; }
template <typename T>
T pow(T num, unsigned short power) { 
	if (power == 0) return T(1);
	else {
		T product = 1;
		for (short i = 1; i <= power; i++) {
			product *= num;
		}
		return product;
	}
}

// get image size of png
// courtesy of Aliaksei Luferau
// https://stackoverflow.com/a/69105584
inline vec2D get_png_image_dimensions(std::string& file_path)
{
	vec2D size;
	unsigned char buf[8];

	std::ifstream in(file_path);
	in.seekg(16);
	in.read(reinterpret_cast<char*>(&buf), 8);

	size.x = (buf[0] << 24) + (buf[1] << 16) + (buf[2] << 8) + (buf[3] << 0);
	size.y = (buf[4] << 24) + (buf[5] << 16) + (buf[6] << 8) + (buf[7] << 0);

	return size;
}

#endif // !util_h
