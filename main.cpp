#include "modules/UI/constants.hpp"
#include "modules/UI/window.hpp"
#include "modules/UI/button.hpp"
#include "modules/UI/image.hpp"
#include "modules/UI/fpsCounter.hpp"

#include "modules/engine/camera.hpp"
#include "modules/UI/cameraView.hpp"

using namespace owo;

int main(int argc, char const *argv[])
{
    CONST::init();
    Window win("FullBowody", 1280, 720);
    win.addElement(new FPSCounter());

    win.startUpdating();

    while (win.isOpen())
    {
        win.processEvents();
        win.refresh();
    }

    win.stopUpdating();

    return 0;
}