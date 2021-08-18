#pragma once
#include <thread>
#include <string>
#include <fstream>
#include <streambuf>
#include "./basics.hpp"

namespace CONSTANT
{
    /** @brief Number of body joints in the app*/
    const unsigned char NB_JOINTS = 20;
    // body joints
    const unsigned char JOINT_HEAD = 0;
    const unsigned char JOINT_EYE_R = 1;
    const unsigned char JOINT_EYE_L = 2;
    const unsigned char JOINT_NECK = 3;
    const unsigned char JOINT_SHOULDER_R = 4;
    const unsigned char JOINT_ELBOW_R = 5;
    const unsigned char JOINT_WRIST_R = 6;
    const unsigned char JOINT_HAND_R = 7;
    const unsigned char JOINT_HIP_R = 8;
    const unsigned char JOINT_KNEE_R = 9;
    const unsigned char JOINT_ANKLE_R = 10;
    const unsigned char JOINT_FEET_R = 11;
    const unsigned char JOINT_SHOULDER_L = 12;
    const unsigned char JOINT_ELBOW_L = 13;
    const unsigned char JOINT_WRIST_L = 14;
    const unsigned char JOINT_HAND_L = 15;
    const unsigned char JOINT_HIP_L = 16;
    const unsigned char JOINT_KNEE_L = 17;
    const unsigned char JOINT_ANKLE_L = 18;
    const unsigned char JOINT_FEET_L = 19;
    
    /** @brief Number of connections between the body joints */
    const unsigned char NB_CONNECTIONS = 19;
    /** @brief Connections between each joint of the body for a wired representation */
    const unsigned char POSE_CONNECTIONS[NB_CONNECTIONS][2] = {
        {JOINT_HEAD, JOINT_NECK},
        {JOINT_HEAD, JOINT_EYE_R},
        {JOINT_HEAD, JOINT_EYE_L},
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

typedef struct Point3f
{
    float x, y, z;
    Point3f(float x, float y, float z): x(x), y(y), z(z) {}
    Point3f(): x(0), y(0), z(0) {}
} Point3f;

Point3f bodyPos[CONSTANT::NB_JOINTS];

class Receiver
{
private:
    std::thread receiver;
    bool running;

public:
    Receiver()
    {
        
    }

    void Start()
    {
        this->running = false;
        for(int i = 0; i < CONSTANT::NB_JOINTS; i++)
            bodyPos[i] = Point3f();
        if (this->running) return;
        this->running = true;
        this->receiver = std::thread(&Receiver::_check_for_data, this);
    }

    void Stop()
    {
        if (this->running)
        {
            this->running = false;
            this->receiver.join();
        }
    }

    void _check_for_data()
    {
        while (this->running)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            
            std::ifstream stream("C:/Users/FurWaz/Documents/GitHub/FullBowody/resources/bodyPos.txt");
            std::string data((std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());
            stream.close();
            unsigned short cursor = 0;
            unsigned short index = 0;
            // get the body points from the string
            while (cursor < data.size() && index < CONSTANT::NB_JOINTS)
            {
                try 
                {
                    // get x coordonnate
                    std::string nbrX = "";
                    while (data[cursor] != '|')
                        nbrX += data[cursor++];
                    cursor++;

                    // get y coordonnate
                    std::string nbrY = "";
                    while (data[cursor] != '|')
                        nbrY += data[cursor++];
                    cursor++;

                    // get z coordonnate
                    std::string nbrZ = "";
                    while (data[cursor] != '\n')
                        nbrZ += data[cursor++];
                    cursor++;

                    // add coordonnates to vector
                    float coordX = std::stof(nbrX.c_str());
                    float coordZ = std::stof(nbrZ.c_str());
                    float coordY = std::stof(nbrY.c_str());
                    bodyPos[index] = Point3f(coordX, coordZ, coordY);
                } catch (std::exception &e) {break;}
                index++;
            }
            bodyPos[CONSTANT::JOINT_HAND_L].x -= bodyPos[CONSTANT::JOINT_HEAD].x;
            bodyPos[CONSTANT::JOINT_HAND_L].y -= bodyPos[CONSTANT::JOINT_HEAD].y;
            bodyPos[CONSTANT::JOINT_HAND_L].z -= bodyPos[CONSTANT::JOINT_HEAD].z;
            bodyPos[CONSTANT::JOINT_HAND_R].x -= bodyPos[CONSTANT::JOINT_HEAD].x;
            bodyPos[CONSTANT::JOINT_HAND_R].y -= bodyPos[CONSTANT::JOINT_HEAD].y;
            bodyPos[CONSTANT::JOINT_HAND_R].z -= bodyPos[CONSTANT::JOINT_HEAD].z;
            bodyPos[CONSTANT::JOINT_HAND_L].x = -bodyPos[CONSTANT::JOINT_HAND_L].x;
            bodyPos[CONSTANT::JOINT_HAND_R].x = -bodyPos[CONSTANT::JOINT_HAND_R].x;
        }
    }

    ~Receiver()
    {
        this->Stop();
    }
};