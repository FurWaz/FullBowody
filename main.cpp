#include "modules/constants.hpp"
#include "modules/UI/window.hpp"
#include "modules/UI/button.hpp"
#include "modules/UI/image.hpp"
#include "modules/UI/fpsCounter.hpp"
#include "modules/UI/label.hpp"
#include "modules/UI/input.hpp"

#include "modules/engine/camera.hpp"
#include "modules/UI/cameraView.hpp"

using namespace owo;

int main(int argc, char const *argv[])
{
    CONST::init();
    Window win("FullBowody", 1280, 660);

    Camera* cam = new Camera();
    CameraView* camView = new CameraView(cam, sf::Vector2i(0, 1), sf::Vector2i(250, win.getHeight()/2-2));
    win.addElement(camView);
    win.addElement(camView->getImage());
    win.addElement(camView->getInput());

    Camera* cam2 = new Camera();
    CameraView* camView2 = new CameraView(cam2, sf::Vector2i(0, win.getHeight()/2+1), sf::Vector2i(250, win.getHeight()/2-2));
    win.addElement(camView2);
    win.addElement(camView2->getImage());
    win.addElement(camView2->getInput());

    win.startUpdating();

    while (win.isOpen())
    {
        win.processEvents();
        win.refresh();
    }

    win.stopUpdating();

    return 0;
}