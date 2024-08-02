#include "object.h"

Object::Object(vec2D initPos, float initMass, float initRadius, vec2D initVel,
	vec2D initAccel, olc::Pixel initColor) {

	pos = initPos;
	mass = initMass;
	radius = initRadius;
	vel = initVel;
	accel = initAccel;

}

Object::~Object() {}

void Object::Update(float& fElapsedTime, float& gravity, float &relativeGroundY) {
	forces.clear();
	netForce = { 0, 0 };

	forces.push_back({ 0, mass * gravity }); // force of gravity 
	
	for (auto& f : forces) {
		netForce += f;
	}

	accel = netForce / mass;

	vec2D deltaVel = accel * fElapsedTime;
	vel += deltaVel;
	speed = vel.mag();

	vec2D disp = vel * fElapsedTime;
	pos += disp;

	momentum = vel * mass;

	float groundDispY = std::abs(relativeGroundY - pos.y);
	kineticEnergy = 0.5 * mass * speed * speed;
	potentialGravEnergy = mass * gravity * groundDispY; 
	// would make more sense to calculate potential grav based on a center of gravity 
	// (vec2D), depends how realistic we want it to be

}

void Object::Draw(olc::PixelGameEngine* engine, int& pixelsPerMeter) {
	engine->FillCircle({ (int)pos.x * pixelsPerMeter, (int)pos.y * pixelsPerMeter}, 
		radius, olc::WHITE);
}