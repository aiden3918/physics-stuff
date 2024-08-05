#pragma once

#ifndef object_h
#define object_h

#include "util.h"
#include "olcPixelGameEngine.h"

class Object {
public:
	Object(vec2D initPos, float initMass = 1.0f, float initRadius = 1.0f, 
		vec2D initVel = {0, 0}, vec2D initAccel = {0, 0}, float initDragCoefficient = 0.0f,
		olc::Pixel initColor = olc::BLACK);
	~Object();

	vec2D pos; // meters
	vec2D vel; // meters/second
	float speed; // meters/second
	vec2D accel; // meters/(second^2)

	vec2D netForce; // newtons
	vec2D momentum; // newton * seconds
	float kineticEnergy; // joules
	float potentialGravEnergy; // joules

	float dragCoefficient; // no unit
	float refArea; // meters^2
	float staticFrictionCoefficient; // newtons/newtons
	float kineticFrictionCoefficient; // newtons/newtons

	float mass; // kilograms
	float density; // meters^3
	float volume; // kilograms/(meters^3)
	float radius; // meters

	std::vector<vec2D> forces;
	
	olc::Pixel color;

	float stopwatch;
	bool reachedFinish = false;

	void Update(float& fElapsedTime, float& gravity, float& relativeGroundY, float& fluidDensity, 
		vec2D& screenSize, float& pixelsPerMeter);
	void Draw(olc::PixelGameEngine* engine, float& pixelsPerMeter);
	void UpdateStopwatch(float& fElapsedTime, float& pixlesPerMeter);
	bool checkPtCollision(vec2D &pt, float& pixelsPerMeter);
	void updateMass(const float& m);
	void updateVolume(const float& v);
};

#endif