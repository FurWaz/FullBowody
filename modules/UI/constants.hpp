#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>

namespace CONST
{
    sf::Color COLOR_BLACK(30, 35, 40);
    sf::Color COLOR_WHITE(255, 255, 255);
    sf::Color COLOR_PRIMARY(0, 128, 192);
    sf::Font FONT;
    
    char NO_CHAR = '~';

    void init()
    {
        if (!FONT.loadFromFile("./resources/fonts/Roboto-Regular.ttf"))
            std::cout << "Error loading default font" << std::endl;
    }

    void EMPTY_CALLBACK()
    {
        
    }
};