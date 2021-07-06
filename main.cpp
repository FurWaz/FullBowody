#include "modules/UI/constants.hpp"
#include "modules/UI/window.hpp"
#include "modules/UI/button.hpp"
#include "modules/UI/fpsCounter.hpp"

int main(int argc, char const *argv[])
{
    CONST::init();
    Window win("FullBowody", 1280, 720);
    win.addElement(new FPSCounter());

    Button * btn = new Button();
    btn->setCallback(&Window::close, &win);
    win.addElement(btn);

    win.startUpdating();

    while (win.isOpen())
    {
        win.processEvents();
        win.refresh();
    }

    win.stopUpdating();

    return 0;
}