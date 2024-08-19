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
        
        _shapeDropdown = new olc::QuickGUI::ListBox(_guiManager, _shapesOptions, { 200.0f, 5.0f }, 
            { 100.0f, 50.0f }); 
        _fluidDensityDropdown = new olc::QuickGUI::ListBox(_guiManager, _fluidsOptions, 
            { 350.0f, 5.0f }, { 100.0f, 50.0f });
        _runBtn = new olc::QuickGUI::Button(_guiManager, "Run", {400.0f, 500.0f}, { 50.0f, 50.0f });
        
        _posInputLabel = new olc::QuickGUI::Label(_guiManager, "Init Pos (m):", { 100.0f, 150.0f },
            { 50.0f, 50.0f });
        _posInputX = new olc::QuickGUI::TextBox(_guiManager, std::to_string(worldSize.x / 2.0f),
            { 200.0f, 150.0f }, { 100.0f, 30.0f });
        _posInputY = new olc::QuickGUI::TextBox(_guiManager, std::to_string(worldSize.y / 2.0f),
            { 350.0f, 150.0f }, { 100.0f, 30.0f });

        _velInputLabel = new olc::QuickGUI::Label(_guiManager, "Init Vel (m/s):", { 100.0f, 200.0f },
            { 50.0f, 50.0f });
        _velInputX = new olc::QuickGUI::TextBox(_guiManager, "0", { 200.0f, 200.0f }, 
            { 100.0f, 30.0f });
        _velInputY = new olc::QuickGUI::TextBox(_guiManager, "0", { 350.0f, 200.0f }, 
            { 100.0f, 30.0f });

        _accelInputLabel = new olc::QuickGUI::Label(_guiManager, "Init Accel (m/s^2):", { 100.0f, 250.0f },
            { 50.0f, 50.0f });
        _accelInputX = new olc::QuickGUI::TextBox(_guiManager, "0", { 200.0f, 250.0f }, 
            { 100.0f, 30.0f });
        _accelInputY = new olc::QuickGUI::TextBox(_guiManager, "0", { 350.0f, 250.0f }, 
            { 100.0f, 30.0f });

        _massLabel = new olc::QuickGUI::Label(_guiManager, "Mass (kg):", { 100.0f, 300.0f },
            { 50.0f, 50.0f });
        _massInput = new olc::QuickGUI::TextBox(_guiManager, "1", { 200.0f, 300.0f },
            { 100.0f, 30.0f });

        _radiusLabel = new olc::QuickGUI::Label(_guiManager, "Radius (m):", { 350.0f, 300.0f },
            { 50.0f, 50.0f });
        _radiusInput = new olc::QuickGUI::TextBox(_guiManager, "1", { 450.0f, 300.0f },
            { 100.0f, 30.0f });

        _textInputsVec = {  _posInputX, _posInputY, _velInputX, _velInputY, _accelInputX,
                            _accelInputY, _massInput, _radiusInput };

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

                if (!_allInputsValid()) {
                    _guiManager.colBorder = olc::RED;
                    return true; // for OnUserUpdate
                }
                _guiManager.colBorder = olc::WHITE;

                std::vector<float> convertedInputs;
                for (olc::QuickGUI::TextBox* i : _textInputsVec) {
                    convertedInputs.push_back(std::stof(i->sText));
                }

                // _textInputsVec = { _posInputX, _posInputY, _velInputX, _velInputY, _accelInputX,
                //     _accelInputY, _massInput, _radiusInput };
                // really scuffed, i know.
                Object testCircle = Object({ convertedInputs[0], convertedInputs[1] },
                    convertedInputs[6], convertedInputs[7], { convertedInputs[2],
                    convertedInputs[3] }, { convertedInputs[4], convertedInputs[5] }, 
                    _dragCoefficients.Sphere
                );
                // testCircle.vel = { 10.0f, 10.0f };
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
                o.Update(fElapsedTime, gravityAccel, screenSize.y, envFluidDensity, 
                    screenSize, pixelsPerMeter);
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

            if (envFluidDensity == _fluidDensities.Vacuum) {
                DrawStringDecal({ 5.0f, 300.0f }, "Translational forces at play:\nGravity",
                    olc::BLACK);
            }
            else {
                DrawStringDecal({ 5.0f, 300.0f }, "Translational forces at play:\nGravity\nDrag\nBuoyancy",
                    olc::BLACK);
            }


            DrawStringDecal({ 5.0f, 350.0f }, "Mass: " + std::to_string(firstObj->mass)
                + " kg", olc::BLACK);
            DrawStringDecal({ 5.0f, 360.0f }, "Volume: " + std::to_string(firstObj->volume)
                + " m^3", olc::BLACK);
            DrawStringDecal({ 5.0f, 370.0f }, "Density: " + std::to_string(firstObj->density)
                + " kg/m^3", olc::BLACK);
            DrawStringDecal({ 5.0f, 380.0f }, "Radius: " + std::to_string(firstObj->radius)
                + " m", olc::BLACK);
            
            // scope deletes automatically, i believe
            // delete firstObj;

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

    olc::QuickGUI::Label* _posInputLabel = nullptr;
    olc::QuickGUI::TextBox* _posInputX = nullptr;
    olc::QuickGUI::TextBox* _posInputY = nullptr;
    olc::QuickGUI::Label* _velInputLabel = nullptr;
    olc::QuickGUI::TextBox* _velInputX = nullptr;
    olc::QuickGUI::TextBox* _velInputY = nullptr;
    olc::QuickGUI::Label* _accelInputLabel = nullptr;
    olc::QuickGUI::TextBox* _accelInputX = nullptr; // probably does nothing lol
    olc::QuickGUI::TextBox* _accelInputY = nullptr; // probably does nothing lol
    olc::QuickGUI::Label* _massLabel = nullptr;
    olc::QuickGUI::TextBox* _massInput = nullptr;
    olc::QuickGUI::Label* _radiusLabel = nullptr;
    olc::QuickGUI::TextBox* _radiusInput = nullptr;

    AppState _appState = AppState::CONFIG;
    
    // probably dont need to be deleted because pointers are only assigned once at start 
    std::vector<olc::QuickGUI::TextBox*> _textInputsVec;

    bool _allInputsValid() {
        for (olc::QuickGUI::TextBox *i : _textInputsVec) {
            if (!isIntOrFloat(i->sText)) return false;
        }
        if (std::stof(_massInput->sText) <= 0.0f) return false;
        if (std::stof(_radiusInput->sText) <= 0.0f) return false;
    }
};

int main()
{
    std::cout << std::stof("2342adsf");
    // std::cout << std::stof("adsfas234.adsf2adsf");
    std::cout << "double range: (" << DBL_MIN << ", " << DBL_MAX << ")" << std::endl;
    std::cout << isIntOrFloat("324") << std::endl;
    std::cout << isIntOrFloat("123.456") << std::endl;
    std::cout << isIntOrFloat("asfd") << std::endl;
    std::cout << isIntOrFloat("2324.234.23") << std::endl;

    App app;
    app.screenSize = { 800, 600 };

    if (app.Construct(app.screenSize.x, app.screenSize.y, 1, 1)) {
        std::cout << "successfully initialized app" << std::endl;
        app.Start();
    }
}