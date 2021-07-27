#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>

namespace CONSTANT
{
    sf::Color COLOR_BLACK_DARK  (44, 62, 80);
    sf::Color COLOR_BLACK_LIGHT (52, 73, 94);
    sf::Color COLOR_GREY_DARK  (127, 140, 141);
    sf::Color COLOR_GREY_LIGHT (149, 165, 166);
    sf::Color COLOR_WHITE_DARK  (189, 195, 199);
    sf::Color COLOR_WHITE_LIGHT (236, 240, 241);
    sf::Color COLOR_RED_DARK  (192, 57, 43);
    sf::Color COLOR_RED_LIGHT (231, 76, 60);
    sf::Color COLOR_BLUE_DARK  (41, 128, 185);
    sf::Color COLOR_BLUE_LIGHT (52, 152, 219);
    sf::Color COLOR_ORANGE_DARK  (230, 126, 34);
    sf::Color COLOR_ORANGE_LIGHT (231, 76, 60);
    sf::Color COLOR_PURPLE_DARK  (192, 57, 43);
    sf::Color COLOR_PURPLE_LIGHT (142, 68, 173);
    sf::Color COLOR_GREEN_DARK  (39, 174, 96);
    sf::Color COLOR_GREEN_LIGHT (46, 204, 113);
    sf::Color COLOR_YELLOW_DARK  (243, 156, 18);
    sf::Color COLOR_YELLOW_LIGHT (241, 196, 15);

    sf::Color COLOR_CLEAR = COLOR_BLACK_DARK;
    sf::Color COLOR_BACK = COLOR_BLACK_LIGHT;
    sf::Color COLOR_FORE = COLOR_WHITE_LIGHT;
    sf::Color COLOR_PRIMARY = COLOR_GREEN_LIGHT;
    sf::Color COLOR_TRANS      (0, 0, 0, 0);
    
    sf::Font FONT;
    
    char NO_CHAR = '~';
    int CAMERA_FPS = 15;

    void init()
    {
        if (!FONT.loadFromFile("./resources/fonts/Roboto-Regular.ttf"))
            std::cout << "Error loading default font" << std::endl;
    }
}