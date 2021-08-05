#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>

template<typename T> T clamp(T val, T min, T max)
{
    if (val < min) return min;
    if (val > max) return max;
    return val;
}

/**
 * @brief Constant namespace, contains all the common variables of the program 
 * such as the colors, etc.
 */
namespace CONSTANT
{
    sf::Color COLOR_BLACK_DARK   (44, 62, 80);
    sf::Color COLOR_BLACK_LIGHT  (52, 73, 94);
    sf::Color COLOR_GREY_DARK    (127, 140, 141);
    sf::Color COLOR_GREY_LIGHT   (149, 165, 166);
    sf::Color COLOR_WHITE_DARK   (189, 195, 199);
    sf::Color COLOR_WHITE_LIGHT  (236, 240, 241);
    sf::Color COLOR_RED_DARK     (192, 57, 43);
    sf::Color COLOR_RED_LIGHT    (231, 76, 60);
    sf::Color COLOR_BLUE_DARK    (41, 128, 185);
    sf::Color COLOR_BLUE_LIGHT   (52, 152, 219);
    sf::Color COLOR_ORANGE_DARK  (230, 126, 34);
    sf::Color COLOR_ORANGE_LIGHT (231, 76, 60);
    sf::Color COLOR_PURPLE_DARK  (192, 57, 43);
    sf::Color COLOR_PURPLE_LIGHT (142, 68, 173);
    sf::Color COLOR_GREEN_DARK   (39, 174, 96);
    sf::Color COLOR_GREEN_LIGHT  (46, 204, 113);
    sf::Color COLOR_YELLOW_DARK  (243, 156, 18);
    sf::Color COLOR_YELLOW_LIGHT (241, 196, 15);

    /** @brief App clear color for background */
    sf::Color COLOR_CLEAR = COLOR_BLACK_DARK;
    /** @brief App's widgets background color */
    sf::Color COLOR_BACK = COLOR_BLACK_LIGHT;
    /** @brief App's foreground color*/
    sf::Color COLOR_FORE = COLOR_WHITE_LIGHT;
    /** @brief App's primary color*/
    sf::Color COLOR_PRIMARY = COLOR_GREEN_LIGHT;
    /** @brief App transparent color*/
    sf::Color COLOR_TRANS(0, 0, 0, 0);

    /** @brief Number of body joints */
    const unsigned char NB_JOINTS = 33;
    /** @brief Number of connections between body joints */
    const unsigned char NB_CONNECTIONS = 35;
    /** @brief Connections between each joint of the body for a wired representation */
    const unsigned char POSE_CONNECTIONS[35][2] = {
        {15, 21},
        {16, 20},
        {18, 20},
        { 3,  7},
        {14, 16},
        {23, 25},
        {10, 9},
        {28, 30},
        {11, 23},
        {27, 31},
        {24, 23},
        { 6,  8},
        {15, 17},
        {24, 26},
        {16, 22},
        { 4,  5},
        { 5,  6},
        {29, 31},
        {12, 24},
        { 0,  1},
        { 1,  2},
        { 0,  4},
        {11, 13},
        {30, 32},
        {28, 32},
        {15, 19},
        {16, 18},
        {25, 27},
        {12, 11},
        {26, 28},
        {12, 14},
        {17, 19},
        { 2,  3},
        {27, 29},
        {13, 15}
    };
    
    /** @brief App's main font */
    sf::Font FONT;
    const char NO_CHAR = '~';

    unsigned short TRACKING_FPS = 20;
    unsigned short CAMERA_FPS = 30;

    unsigned short TRACKING_FPS_DELTA = 0;
    unsigned short CAMERA_FPS_DELTA = 0;

    unsigned short WINDOW_WIDTH = 0;
    unsigned short WINDOW_HEIGHT = 0;

    /**
     * @brief Initialises the common variables such as the fonts
     */
    void init()
    {
        if (!FONT.loadFromFile("./resources/fonts/Roboto-Regular.ttf"))
            std::cout << "Error loading default font" << std::endl;
        
        TRACKING_FPS_DELTA = 1000 / TRACKING_FPS;
        CAMERA_FPS_DELTA = 1000 / CAMERA_FPS;
    }
}