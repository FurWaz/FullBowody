#include "modules/constants.hpp"
#include "modules/UI/window.hpp"
#include "modules/UI/sceneGenerator.hpp"

#include "./modules/OpenXR/init.hpp"

using namespace owo;

int main(int argc, char const *argv[])
{
    CONSTANT::init();

    init_openxr();

    // Window win("FullBowody", sf::VideoMode::getDesktopMode().width*0.75f, sf::VideoMode::getDesktopMode().height*0.75f);
    Window win("FullBowody", 1280, 720);
    SceneGenerator::GenerateDefaultScene(win);

    win.startUpdating();
    while (win.isOpen())
    {
        win.processEvents();
        win.refresh();
    }
    win.stopUpdating();

    shutdown_openxr();

    return 0;
}