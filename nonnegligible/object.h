#pragma once

#ifndef object_h
#define object_h

#include "util.h"
#include "olcPixelGameEngine.h"

class Object {
public:
	Object(vec2D initPos, float initMass = 1.0f, float initRadius = 3.0f, 
		vec2D initVel = {0, 0}, vec2D initAccel = {0, 0}, float initDragCoefficient = 0.47,
		olc::Pixel initColor = olc::BLACK);
	~Object();

	vec2D pos; // meters
	vec2D vel; // meters/second
	float speed; // meters/second
	vec2D accel; // meters/(second^2)

	float radius; // meters

	vec2D netForce; // newtons
	vec2D momentum; // newton * seconds
	float kineticEnergy; // joules
	float potentialGravEnergy; // joules

	float dragCoefficient; 
	float refArea; // meters^2
	float volume;
	float staticFrictionCoefficient; 
	float kineticFrictionCoefficient;

	float mass; // kilograms
	
	std::vector<vec2D> forces;
	
	olc::Pixel color;

	void Update(float& fElapsedTime, float& gravity, float& relativeGroundY, float& fluidDensity);
	void Draw(olc::PixelGameEngine* engine, int& pixelsPerMeter);
};

#endif