#include "modules/constants.hpp"
#include "modules/UI/window.hpp"
#include "modules/UI/sceneGenerator.hpp"

using namespace owo;

int main(int argc, char const *argv[])
{
    CONSTANT::init();

    Window win("FullBowody", sf::VideoMode::getDesktopMode().width*0.8f, sf::VideoMode::getDesktopMode().height*0.8f);
    SceneGenerator::GenerateDefaultScene(win);
    // SceneGenerator::GenerateTestScene(win);

    win.startUpdating();
    while (win.isOpen())
    {
        win.processEvents();
        win.refresh();
    }
    win.stopUpdating();

    return 0;
}