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

std::map<std::string, float> fluidDensities{
    {"Air (STP)", 1.292f},
    {"Vacuum", 0.0f},
    {"Water (STP)", 997.0f}
};

std::map<std::string, float> dragCoefficients{
    {"Sphere",0.47f},
    {"Cone", 0.50f},
    {"Airfoil", 0.045f},
    {"Flat Plate", 1.28f},
    {"Cube", 1.05f}
};

const enum class AppState {CONFIG, RUN};

class App : public olc::PixelGameEngine {
public:
    App() {
        sAppName = "air resistance test";
    }

    bool OnUserCreate() override{

        finishLine = 500.0f / pixelsPerMeter;

        worldSize = screenSize / pixelsPerMeter;

        // envFluidDensity = fluidDensities.Vacuum;
        envFluidDensity = _fluidDensities.AirSTP;
        
        _shapeDropdown = new olc::QuickGUI::ListBox(_guiManager, _shapesOptions, { 400.0f, 5.0f }, 
            { 100.0f, 50.0f }); 
        _fluidDensityDropdown = new olc::QuickGUI::ListBox(_guiManager, _fluidsOptions, 
            { 550.0f, 5.0f }, { 100.0f, 50.0f });
        _runBtn = new olc::QuickGUI::Button(_guiManager, "Run", {400.0f, 500.0f}, { 50.0f, 50.0f });


        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override {

        if (envFluidDensity == _fluidDensities.AirSTP) Clear(olc::GREY);
        else if (envFluidDensity == _fluidDensities.Vacuum) Clear(olc::DARK_GREY);
        else if (envFluidDensity == _fluidDensities.WaterSTP) Clear(olc::BLUE);

        switch (_appState) {
        case AppState::CONFIG: {

            _guiManager.Update(this);
            _guiManager.Draw(this);

            if (_runBtn->bPressed) {
                Object testCircle = Object({ 0.5f * screenSize.x / pixelsPerMeter, 0 });
                testCircle.updateMass(2.0f);
                testCircle.dragCoefficient = _dragCoefficients.Sphere;
                testCircle.vel = { 10.0f, 10.0f };
                objects.push_back(testCircle);

                int fluidDensityInt = _fluidDensityDropdown->nSelectedItem;
                std::string _fluidDensityStr = _fluidsOptions[fluidDensityInt];
                envFluidDensity = fluidDensities[_fluidDensityStr];
                std::cout << envFluidDensity << std::endl;

                int dragCoeffInt = _shapeDropdown->nSelectedItem;
                std::string _shapeStr = _shapesOptions[dragCoeffInt];
                testCircle.dragCoefficient = dragCoefficients[_shapeStr];
                std::cout << testCircle.dragCoefficient << std::endl;

                _appState = AppState::RUN;
            }

            break;
        }
        case AppState::RUN: {
            vec2D mousePos = { (float)GetMouseX(), (float)GetMouseY() };

            DrawStringDecal({ 5.0f, 5.0f }, "Screen: " + std::to_string((int)worldSize.x) + "m x " +
                std::to_string((int)worldSize.y) + "m", olc::BLACK);

            if (objects.size() == 0) break;

            for (Object& o : objects) {
                o.Update(fElapsedTime, gravityAccel, screenSize.y, envFluidDensity, screenSize, 
                    pixelsPerMeter);
                o.Draw(this, pixelsPerMeter);
                o.UpdateStopwatch(fElapsedTime, pixelsPerMeter);
            }

            if (GetMouse(olc::Mouse::LEFT).bHeld) {
                for (Object& o : objects) {
                    if (o.checkPtCollision(mousePos, pixelsPerMeter)) {
                        vec2D oldPos = o.pos;
                        vec2D newPos = mousePos / pixelsPerMeter;
                        o.pos = newPos;
                        vec2D newVel = (newPos - oldPos) / (fElapsedTime * 2.0f);
                        o.vel = newVel;
                        o.accel = { 0, 0 };
                    }
                }
            }


            if (GetKey(olc::SPACE).bPressed) {
                for (Object& o : objects) {
                    std::cout << "pos: (" << o.pos.x << ", " << o.pos.y << ")" << std::endl;
                    std::cout << "vel: (" << o.vel.x << ", " << o.vel.y << ")" << std::endl;
                    std::cout << "accel: (" << o.accel.x << ", " << o.accel.y << ")" << std::endl << std::endl;
                    
                }
            }

            Object* firstObj = &objects[0];
            DrawStringDecal({ 5.0f, 100.0f }, "pos: (" + std::to_string(firstObj->pos.x) +
                ", " + std::to_string(firstObj->pos.y) + ")", olc::BLACK);
            DrawStringDecal({ 5.0f, 110.0f }, "vel: (" + std::to_string(firstObj->vel.x) +
                ", " + std::to_string(firstObj->vel.y) + ")", olc::BLACK);
            DrawStringDecal({ 5.0f, 120.0f }, "accel: (" + std::to_string(firstObj->accel.x) +
                ", " + std::to_string(firstObj->accel.y) + ")", olc::BLACK);
            DrawStringDecal({ 5.0f, 130.0f }, "stopwatch: " + std::to_string(firstObj->stopwatch) +
                " sec", olc::BLACK);

            DrawLineDecal({ 0.0f, 500.0f }, { (float)screenSize.x, 500.0f }, olc::GREEN);
            DrawStringDecal({ 5.0f, 510.0f }, std::to_string(finishLine) + " m from top", olc::GREEN);

            DrawStringDecal({ 5.0f, 200.0f }, "Time to reach " + std::to_string(finishLine) + "m: " +
                std::to_string(firstObj->stopwatch) + "sec", olc::BLACK);

            DrawStringDecal({ 5.0f, 300.0f }, "Forces at play:\nGravity\nDrag\nBuoyancy");

            if (GetKey(olc::R).bPressed) {
                objects.clear();
                _appState = AppState::CONFIG;
            }

            break;
        }
        }


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

    FluidDensities _fluidDensities;
    DragCoefficients _dragCoefficients;

    olc::QuickGUI::Manager _guiManager;
    std::vector<std::string> _fluidsOptions = { "Vacuum", "Air (STP)", "Water (STP)" };
    olc::QuickGUI::ListBox* _fluidDensityDropdown = nullptr;
    std::vector<std::string> _shapesOptions = { "Sphere", "Cone", "Airfoil", "Flat Plate", "Bullet", "Cone" };
    olc::QuickGUI::ListBox* _shapeDropdown = nullptr;
    olc::QuickGUI::Button* _runBtn = nullptr;

    AppState _appState = AppState::CONFIG;
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