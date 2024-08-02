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

	// reference area is the projected frontal area, not always its cross-sectional area
	refArea = PI * radius * radius;
	volume = (4 / 3) * radius * radius * radius;

	stopwatch = 0.0f;

}

Object::~Object() {}

void Object::Update(float& fElapsedTime, float& gravity, float &relativeGroundY, float& fluidDensity) {
	forces.clear();
	netForce = { 0, 0 };

	// positive is to the right and down
	vec2D gravityForce = { 0, mass * gravity };
	forces.push_back(gravityForce); // force of gravity (f_g = mg)

	// drag force (f_d = 0.5*p*C*A*v^2)

	vec2D velSquared = vec2DElementwiseMult(vel, vel);
	vec2D dragForce = velSquared * (0.5 * fluidDensity * dragCoefficient);
	if (vel.x > 0) dragForce.x *= -1.0f; // opposes motion
	if (vel.y > 0) dragForce.y *= -1.0f;
	forces.push_back(dragForce); 
	
	for (vec2D& f : forces) netForce += f;

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
		radius * pixelsPerMeter, color);
}

void Object::UpdateStopwatch(float& fElapsedTime, float& pixlesPerMeter) {
	float maxDispY = 500.0f / pixlesPerMeter;
	if (pos.y <= maxDispY && !reachedFinish) stopwatch += fElapsedTime;
	else if (pos.y > maxDispY) {
		vel.y *= -1.0f;
		reachedFinish = true;
	}
}

bool checkPtCircleCollision(vec2D& pt, Object& circle) {
	vec2D mouseDisp = circle.pos - pt;
	float mouseHyp = mouseDisp.mag();
	return (circle.radius <= mouseHyp) ? true : false;
}