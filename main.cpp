#include "modules/constants.hpp"
#include "modules/UI/window.hpp"
#include "modules/UI/button.hpp"
#include "modules/UI/image.hpp"
#include "modules/UI/fpsCounter.hpp"
#include "modules/UI/label.hpp"
#include "modules/UI/input.hpp"
#include "modules/UI/cameraView.hpp"

#include "modules/engine/camera.hpp"
#include "modules/engine/tracker.hpp"

using namespace owo;

int main(int argc, char const *argv[])
{
    CONST::init();
    Window win("FullBowody", 1280, 660);

    int cameraWidth = 250;

    Camera* cam = new Camera();
    cam->openSource("http://192.168.1.76:8080/video");
    cam->loadCalibration("./out.txt");
    cam->setDebugMode(true);
    CameraView* camView = new CameraView(cam, sf::Vector2i(10, 10), sf::Vector2i(cameraWidth, win.getHeight()-20));
    win.addElement(camView);
    win.addElement(camView->getImage());
    win.addElement(camView->getInput());

    Camera* cam2 = new Camera();
    CameraView* camView2 = new CameraView(cam2, sf::Vector2i(win.getWidth()-cameraWidth-10, 10), sf::Vector2i(cameraWidth, win.getHeight()-20));
    win.addElement(camView2);
    win.addElement(camView2->getImage());
    win.addElement(camView2->getInput());

    Button* btn = new Button(
        "Start tracking",
        sf::Vector2i(win.getWidth()/2-100, 10), sf::Vector2i(200, 50),
        CONST::COLOR_BACKGROUND, CONST::COLOR_PRIMARY
    );
    win.addElement(btn);

    win.startUpdating();

    while (win.isOpen())
    {
        win.processEvents();
        win.refresh();
        std::cout << "Position: " << cam->getPosition()*100 << "Rotation: " << cam->getRotation()*(180/3.1415926) << std::endl;
    }

    win.stopUpdating();

    return 0;
}