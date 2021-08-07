#include "modules/constants.hpp"
#include "modules/UI/window.hpp"
#include "modules/sceneManager.hpp"
#include "modules/engine/cameraCalibrator.hpp"

using namespace owo;

int main(int argc, char const *argv[])
{
    CONSTANT::init();
    Window win("FullBowody", sf::VideoMode::getDesktopMode().width*0.8f, sf::VideoMode::getDesktopMode().height*0.8f);
    CameraCalibrator::setWindow(&win);

    SceneManager::init();
    SceneManager::setWindow(&win);
    SceneManager::GenerateDefaultScene();

    win.startUpdating();
    while (win.isOpen())
    {
        win.processEvents();
        win.refresh();
    }
    win.stopUpdating();

    return 0;
}