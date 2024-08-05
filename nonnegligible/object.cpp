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
	
	// sphere
	refArea = PI * square(radius);
	volume = (4.0f / 3.0f) * cube(radius);
	
	density = mass / volume;

}

Object::~Object() {}

void Object::Update(float& fElapsedTime, float& gravity, double &relativeGroundY, 
	float& fluidDensity, vec2D& screenSize, float& pixelsPerMeter) 
{
	forces.clear();
	netForce = { 0, 0 };

	// positive is to the right and down
	vec2D gravityForce = { 0, mass * gravity };
	forces.push_back(gravityForce); // force of gravity (f_g = mg)

	// NOTE: drag coefficient (and even the drag formula itself) differs depending on velocity
	// consult the reynolds number 
	// DEBUG: water at stp launches ball into -oblivion on both axes (not bugged, but calculations
	// might be wrong)
	
	// reference area is the projected frontal area, not always its cross-sectional area
	vec2D velSquared = vec2DElementwiseMult(vel, vel);
	vec2D dragForce = velSquared * (0.5 * fluidDensity * dragCoefficient);
	if (vel.x > 0) dragForce.x *= -1.0f; // opposes motion
	if (vel.y > 0) dragForce.y *= -1.0f;
	forces.push_back(dragForce); // drag force (f_d = 0.5*p*C*A*v^2)

	float displacedVolume = 0.0f;
	if (pos.y > radius) displacedVolume = volume;
	//volume of section of circle (derived by integration): 
	//V = PI * [(r^2)(B - A) + (1/3)(A^3 - B^3)]
	//where A = starting point, B = ending point, R = radius
	else if (pos.y > -radius) {
		float A = -pos.y;
		float B = radius;
		displacedVolume = 
			PI * ((square<double>(radius) * (B - A)) + ((cube<double>(A) - cube<double>(B)) / 3.0f));
	}
	vec2D buoyancyForce = {0, -fluidDensity * gravity * displacedVolume}; // buoyancy force (f_b = -pgV)
	forces.push_back(buoyancyForce);

	for (vec2D& f : forces) netForce += f;

	accel = netForce / mass;

	vec2D deltaVel = accel * fElapsedTime;
	vel += deltaVel;
	speed = vel.mag();
	if (pos.x * pixelsPerMeter > screenSize.x || pos.x < 0.0f) vel.x *= -1.0f;

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

bool Object::checkPtCollision(vec2D& pt, float &pixelsPerMeter) {
	vec2D mouseDist = (pt / pixelsPerMeter) - pos;
	float mouseHyp = mouseDist.mag();
	bool colliding = (mouseHyp < radius);
	return colliding;
}

void Object::updateMass(const float& m) {
	mass = m;
	density = mass / volume;
}

void Object::updateVolume(const float& v) {
	volume = v;
	density = mass / volume;
}
