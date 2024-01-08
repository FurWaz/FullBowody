#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>

#define loop(x) for(int i = 0; i < x; i++)
#define unloop(x) for(int i = x-1; i >= 0; i--)

template<typename T> T clamp(T val, T min, T max)
{
    if (val < min) return min;
    if (val > max) return max;
    return val;
}

void saveTexture(sf::Sprite s)
{
    sf::Image im = s.getTexture()->copyToImage();
    im.saveToFile("./tex.png");
}

/**
 * @brief Constant namespace, contains all the common variables of the program 
 * such as the colors, etc.
 */
namespace CONSTANT
{
    sf::Color COLOR_BLACK_DARKER(35, 49, 64);
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

    /** @brief Number of body joints from mediapipe */
    const unsigned char MEDIAPIPE_JOINTS = 33;
    /** @brief Number of connections between mediapipe's body joints */
    const unsigned char NB_MEDIAPIPE_CONNECTIONS = 35;
    /** @brief Connections between each joint of the mediapipe body */
    const unsigned char MEDIAPIPE_CONNECTIONS[NB_MEDIAPIPE_CONNECTIONS][2] = {
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
    const float RAD2DEG = 180.f / 3.1415926f;
    const float DEG2RAD = 3.1415926f / 180.f;

    unsigned short TRACKING_FPS = 20;
    unsigned short CAMERA_FPS = 30;

    unsigned short TRACKING_FPS_DELTA = 0;
    unsigned short CAMERA_FPS_DELTA = 0;

    unsigned short WINDOW_WIDTH = 0;
    unsigned short WINDOW_HEIGHT = 0;

    void VOIDCALLBACK() {}
    void VOIDCALLBACK_BOOL(bool state) {}
    void VOIDCALLBACK_STRING(std::string str) {}

    void updateFPSdelta()
    {
        TRACKING_FPS_DELTA = 1000 / TRACKING_FPS;
        CAMERA_FPS_DELTA = 1000 / CAMERA_FPS;
    }

    /**
     * @brief Initialises the common variables such as the fonts
     */
    void init()
    {
        if (!FONT.loadFromFile("./resources/fonts/Roboto-Regular.ttf"))
            std::cout << "Error loading default font" << std::endl;
        updateFPSdelta();
    }
    
    /** @brief Number of body joints in the app*/
    const unsigned char NB_JOINTS = 19;
    // body joints
    const unsigned char JOINT_HEAD = 0;
    const unsigned char JOINT_NOSE = 1;
    const unsigned char JOINT_NECK = 2;
    const unsigned char JOINT_SHOULDER_R = 3;
    const unsigned char JOINT_ELBOW_R = 4;
    const unsigned char JOINT_WRIST_R = 5;
    const unsigned char JOINT_HAND_R = 6;
    const unsigned char JOINT_HIP_R = 7;
    const unsigned char JOINT_KNEE_R = 8;
    const unsigned char JOINT_ANKLE_R = 9;
    const unsigned char JOINT_FEET_R = 10;
    const unsigned char JOINT_SHOULDER_L = 11;
    const unsigned char JOINT_ELBOW_L = 12;
    const unsigned char JOINT_WRIST_L = 13;
    const unsigned char JOINT_HAND_L = 14;
    const unsigned char JOINT_HIP_L = 15;
    const unsigned char JOINT_KNEE_L = 16;
    const unsigned char JOINT_ANKLE_L = 17;
    const unsigned char JOINT_FEET_L = 18;
    
    /** @brief Number of connections between the body joints */
    const unsigned char NB_CONNECTIONS = 18;
    /** @brief Connections between each joint of the body for a wired representation */
    const unsigned char POSE_CONNECTIONS[NB_CONNECTIONS][2] = {
        {JOINT_HEAD, JOINT_NECK},
        {JOINT_HEAD, JOINT_NOSE},
        {JOINT_NECK, JOINT_SHOULDER_R},
        {JOINT_NECK, JOINT_SHOULDER_L},
        {JOINT_SHOULDER_R, JOINT_ELBOW_R},
        {JOINT_SHOULDER_L, JOINT_ELBOW_L},
        {JOINT_ELBOW_R, JOINT_WRIST_R},
        {JOINT_ELBOW_L, JOINT_WRIST_L},
        {JOINT_WRIST_R, JOINT_HAND_R},
        {JOINT_WRIST_L, JOINT_HAND_L},
        {JOINT_NECK, JOINT_HIP_R},
        {JOINT_NECK, JOINT_HIP_L},
        {JOINT_HIP_R, JOINT_KNEE_R},
        {JOINT_HIP_L, JOINT_KNEE_L},
        {JOINT_KNEE_R, JOINT_ANKLE_R},
        {JOINT_KNEE_L, JOINT_ANKLE_L},
        {JOINT_ANKLE_R, JOINT_FEET_R},
        {JOINT_ANKLE_L, JOINT_FEET_L}
    };
}