#include "modules/UI/constants.hpp"
#include "modules/UI/window.hpp"
#include "modules/UI/button.hpp"

int main(int argc, char const *argv[])
{
    CONST::init();
    Window win("FullBowody", 1280, 720);

    win.addElement(
        new Button(
            "Quit",
            sf::Vector2i(20, 20), sf::Vector2i(140, 40),
            CONST::COLOR_BLACK, CONST::COLOR_PRIMARY
        )
    );

    while (win.isOpen())
    {
        win.processEvents();
        win.update();
        win.refresh();
    }

    return 0;
}