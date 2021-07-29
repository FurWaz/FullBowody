#include "modules/constants.hpp"
#include "modules/UI/window.hpp"
#include "modules/UI/sceneGenerator.hpp"

using namespace owo;

int main(int argc, char const *argv[])
{
    CONSTANT::init();

    Window win("FullBowody", 1920, 1080);
    SceneGenerator::GenerateDefaultScene(win);
    win.startUpdating();

    while (win.isOpen())
    {
        win.processEvents();
        win.refresh();
    }

    win.stopUpdating();

    return 0;
}