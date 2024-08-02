#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

#define OLC_PGEX_QUICKGUI
#include "olcPGEX_QuickGUI.h"

#include "util.h"
#include "object.h"
#include <iostream>

// higher fluid density, more drag
const struct FluidDensities {
    const float AirSTP = 1.292f;
    const float Vacuum = 0.0f;
    const float WaterSTP = 997.0f;
};

// smaller drag coefficient = more aerodynamic
// all are estimations of shapes when oriented to be the most aerodynamic
const struct DragCoefficients {
    const float Sphere = 0.47f;
    const float Cone = 0.50f;
    const float Airfoil = 0.045f;
    const float FlatPlate = 1.28f;
    const float Bullet = 0.295;
    const float Cube = 1.05f;
};

class App : public olc::PixelGameEngine {
public:
    App() {
        sAppName = "air resistance test";
    }

    bool OnUserCreate() override{
        Object testCircle = Object({ 0.5f * screenSize.x / pixelsPerMeter, 0 });
        testCircle.dragCoefficient = dragCoefficients.Sphere;
        objects.push_back(testCircle);

        envFluidDensity = fluidDensities.AirSTP;
        finishLine = 500.0f / pixelsPerMeter;

        worldSize = screenSize / pixelsPerMeter;

        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override {
        Clear(olc::GREY);

        DrawStringDecal({ 5.0f, 5.0f }, "Screen: " + std::to_string((int)worldSize.x) + "m x " +
            std::to_string((int)worldSize.y) + "m", olc::BLACK);
        
        for (Object& o : objects) {
            o.Update(fElapsedTime, gravityAccel, screenSize.y, envFluidDensity);
            o.Draw(this, pixelsPerMeter);
            o.UpdateStopwatch(fElapsedTime, pixelsPerMeter);
        }

        //if (GetKey(olc::SPACE).bPressed) {
        //    for (Object& o : objects) {
        //        std::cout << "pos: (" << o.pos.x << ", " << o.pos.y << ")" << std::endl;
        //        std::cout << "vel: (" << o.vel.x << ", " << o.vel.y << ")" << std::endl;
        //        std::cout << "accel: (" << o.accel.x << ", " << o.accel.y << ")" << std::endl << std::endl;
        //        
        //    }
        //}

        Object* firstObj = &objects[0];
        DrawStringDecal({ 5.0f, 100.0f }, "pos: (" + std::to_string(firstObj->pos.x) +
            ", " + std::to_string(firstObj->pos.y) + ")", olc::BLACK);
        DrawStringDecal({ 5.0f, 110.0f }, "vel: (" + std::to_string(firstObj->vel.x) +
            ", " + std::to_string(firstObj->vel.y) + ")", olc::BLACK);
        DrawStringDecal({ 5.0f, 120.0f }, "accel: (" + std::to_string(firstObj->accel.x) +
            ", " + std::to_string(firstObj->accel.y) + ")", olc::BLACK);
        DrawStringDecal({ 5.0f, 130.0f }, "stopwatch: " + std::to_string(firstObj->stopwatch) + 
            " sec", olc::BLACK);

        DrawLineDecal({ 0.0f, 500.0f }, { screenSize.x, 500.0f }, olc::GREEN);
        DrawStringDecal({ 5.0f, 510.0f }, std::to_string(finishLine) + " m from top", olc::GREEN);

        DrawStringDecal({ 5.0f, 200.0f }, "Time to reach " + std::to_string(finishLine) + "m: " +
            std::to_string(firstObj->stopwatch) + "sec", olc::BLACK);

        return true;
    }

    bool OnUserDestroy() override {
        return true;
    }

    vec2D screenSize;
    vec2D worldSize;
    float envFluidDensity;
    // float airFluidDensity = 0.0f;

    float finishLine;

private:
    std::vector<Object> objects;

    float pixelsPerMeter = 20.0f; // conversion from screen space to world space
    float gravityAccel = 9.8f;

    FluidDensities fluidDensities;
    DragCoefficients dragCoefficients;
};

int main()
{
    App app;
    app.screenSize = { 800, 600 };

    if (app.Construct(app.screenSize.x, app.screenSize.y, 1, 1)) {
        std::cout << "successfully initialized app" << std::endl;
        app.Start();
    }
}