#include "modules/constants.hpp"
#include "modules/UI/window.hpp"
#include "modules/UI/sceneGenerator.hpp"

#include "./modules/OpenXR/core.hpp"

using namespace owo;

int main(int argc, char const *argv[])
{
    CONSTANT::init();

    // Window win("FullBowody", sf::VideoMode::getDesktopMode().width*0.75f, sf::VideoMode::getDesktopMode().height*0.75f);
    Window win("FullBowody", 1280, 720);
    SceneGenerator::GenerateDefaultScene(win);

    OpenXRCore core;
    core.start();

    win.startUpdating();
    while (win.isOpen())
    {
        win.processEvents();
        win.refresh();
    }
    win.stopUpdating();

    core.stop();

    return 0;
}