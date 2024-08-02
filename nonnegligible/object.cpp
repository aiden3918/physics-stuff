#include "object.h"

Object::Object(vec2D initPos, float initMass, float initRadius, vec2D initVel,
	vec2D initAccel, float initDragCoefficient, olc::Pixel initColor) {

	pos = initPos;
	mass = initMass;
	radius = initRadius;
	vel = initVel;
	accel = initAccel;
	dragCoefficient = initDragCoefficient;
	color = initColor;

}

Object::~Object() {}

void Object::Update(float& fElapsedTime, float& gravity, float &relativeGroundY, float& fluidDensity) {
	forces.clear();
	netForce = { 0, 0 };

	// positive is to the right and down
	vec2D gravityForce = { 0, mass * gravity };
	forces.push_back(gravityForce); // force of gravity (f_g = mg)
	// reference area is the projected frontal area, not always its cross-sectional area
	refArea = PI * radius * radius;
	vec2D velSquared = vec2DElementwiseMult(vel, vel);
	vec2D dragForce = velSquared * (0.5 * fluidDensity * dragCoefficient);
	dragForce *= -1.0f; // opposes motion
	forces.push_back(dragForce); // drag force (f_d = 0.5*p*C*A*v^2)
	
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

void Object::Draw(olc::PixelGameEngine* engine, float& pixelsPerMeter) {
	engine->FillCircle({ (int)(pos.x * pixelsPerMeter), (int)(pos.y * pixelsPerMeter) }, 
		radius, color);
}

void Object::UpdateStopwatch(float& fElapsedTime) {
	if (pos.y < 50.0f) stopwatch += fElapsedTime;
}

bool checkPtCircleCollision(vec2D& pt, Object& circle) {
	vec2D mouseDisp = circle.pos - pt;
	float mouseHyp = mouseDisp.mag();
	return (circle.radius <= mouseHyp) ? true : false;
}