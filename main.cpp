#include "modules/constants.hpp"
#include "modules/UI/window.hpp"
#include "modules/sceneManager.hpp"
#include "modules/files.hpp"
#include "modules/engine/cameraCalibrator.hpp"

using namespace owo;

int main(int argc, char const *argv[])
{
    CONSTANT::init();
    // Window win(
    //     "FullBowody",
    //     std::max(sf::VideoMode::getDesktopMode().width*0.7f, 1280.f),
    //     std::max(sf::VideoMode::getDesktopMode().height*0.7f, 720.f)
    // );
    Window win(
        "FullBowody",
        1280.f,
        720.f
    );
    CameraCalibrator::setWindow(&win);
    Files::setWindow(&win);

    SceneManager::setWindow(&win);
    SceneManager::GenerateDefaultScene();
    win.setBodyPos(&SceneManager::bp);

    win.startUpdating();
    while (win.isOpen())
    {
        win.processEvents();
        win.refresh();
    }
    win.stopUpdating();

    return 0;
}