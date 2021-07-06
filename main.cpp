#include "modules/UI/constants.hpp"
#include "modules/UI/window.hpp"
#include "modules/UI/button.hpp"
#include "modules/UI/fpsCounter.hpp"

int main(int argc, char const *argv[])
{
    CONST::init();
    Window win("FullBowody", 1280, 720);
    win.addElement(new FPSCounter());

    Button* btn = new Button(
        "Quit",
        sf::Vector2i(win.getWidth()-120, 0), sf::Vector2i(120, 50),
        CONST::COLOR_BLACK, CONST::COLOR_PRIMARY
    );
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