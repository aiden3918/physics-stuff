#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

#define OLC_PGEX_QUICKGUI
#include "olcPGEX_QuickGUI.h"

#include "util.h"
#include "object.h"
#include <iostream>

class App : public olc::PixelGameEngine {
public:
    App() {
        sAppName = "air resistance test";
    }

    bool OnUserCreate() override{
        Object testCircle = Object({ 40, 1 });
        objects.push_back(testCircle);

        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override {
        Clear(olc::WHITE);
        
        for (Object& o : objects) {
            o.Update(fElapsedTime, gravityAccel, screenSize.y);
            o.Draw(this, pixelsPerMeter);
        }

        if (GetKey(olc::SPACE).bPressed) {
            for (Object& o : objects) {
                std::cout << "(" << o.pos.x << ", " << o.pos.y << ")" << std::endl;
            }
        }


        return true;
    }

    bool OnUserDestroy() override {
        return true;
    }

    vec2D screenSize;
private:
    std::vector<Object> objects;

    int pixelsPerMeter = 10; // conversion from screen space to world space
    float gravityAccel = 9.8f;
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