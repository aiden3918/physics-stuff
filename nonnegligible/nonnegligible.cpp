#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

#define OLC_PGEX_QUICKGUI
#include "olcPGEX_QuickGUI.h"

#include "util.h"
#include <iostream>

class App : public olc::PixelGameEngine {
public:
    bool OnUserCreate() override{
        return true;
    }

    bool OnUserUpdate() {
        return true;
    }

    bool OnUserDestroy() override {
        return true;
    }
};

int main()
{
    App app;
    if (app.Construct(800, 600, 1, 1)) {
        std::cout << "successfully initialized app" << std::endl;
        app.Start();
    }
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
