#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

#define OLC_PGEX_QUICKGUI
#include "olcPGEX_QuickGUI.h"

#include "util.h"
#include "object.h"
#include <iostream>

const struct FluidDensities {
    float AirSTP = 1.292f;
    float Vacuum = 0.0f;
    float WaterSTP = 997.0f;
};

class App : public olc::PixelGameEngine {
public:
    App() {
        sAppName = "air resistance test";
    }

    bool OnUserCreate() override{
        Object testCircle = Object({ 0.5f * screenSize.x / pixelsPerMeter, 0 });

        worldSize = screenSize / pixelsPerMeter;
        objects.push_back(testCircle);

        envFluidDensity = fluidDensities.Vacuum;

        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override {
        Clear(olc::WHITE);

        DrawStringDecal({ 5.0f, 5.0f }, "Screen: " + std::to_string((int)worldSize.x) + "m x " +
            std::to_string((int)worldSize.y) + "m", olc::BLACK);
        
        for (Object& o : objects) {
            o.Update(fElapsedTime, gravityAccel, screenSize.y, envFluidDensity);
            o.Draw(this, pixelsPerMeter);
            o.UpdateStopwatch(fElapsedTime);
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
        DrawStringDecal({ 5.0f, 500.0f }, "50 m from top", olc::GREEN);

        DrawStringDecal({ 5.0f, 200.0f }, "Time to reach 50m: " + std::to_string(firstObj->stopwatch),
            olc::BLACK);

        return true;
    }

    bool OnUserDestroy() override {
        return true;
    }

    vec2D screenSize;
    vec2D worldSize;
    float envFluidDensity;
    // float airFluidDensity = 0.0f;

private:
    std::vector<Object> objects;

    float pixelsPerMeter = 10.0f; // conversion from screen space to world space
    float gravityAccel = 9.8f;

    FluidDensities fluidDensities;
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