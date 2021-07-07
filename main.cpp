#include "modules/UI/constants.hpp"
#include "modules/UI/window.hpp"
#include "modules/UI/button.hpp"
#include "modules/UI/image.hpp"
#include "modules/UI/fpsCounter.hpp"

int main(int argc, char const *argv[])
{
    CONST::init();
    Window win("FullBowody", 1280, 720);
    win.addElement(new FPSCounter());

    Button* btn = new Button(
        "Quit",
        sf::Vector2i(win.getWidth()-120, 0), sf::Vector2i(120, 50),
        CONST::COLOR_BLACK, CONST::COLOR_WHITE
    );
    btn->setCallback(&Window::close, &win);
    win.addElement(btn);

    Image* img = new Image("./resources/blur.png", 50, 50, 200, 200);
    win.addElement(img);

    win.startUpdating();

    while (win.isOpen())
    {
        win.processEvents();
        win.refresh();
    }

    win.stopUpdating();

    return 0;
}