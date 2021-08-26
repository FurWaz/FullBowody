#pragma once
#include <thread>
#include <SFML/Network.hpp>
#include "./basics.hpp"
#include "../../../modules/constants.hpp"

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
    const std::string LOGIN_MSG = "[ON]FullBowody-VRPlugin";
    const std::string LOGOUT_MSG = "[OFF]FullBowody-VRPlugin";

    std::string ip;
    unsigned short port;
    sf::UdpSocket socket;

    std::thread receiver;
    bool running;

public:
    Receiver()
    {
        println("Initializing Receiver");
        this->running = false;
        for(int i = 0; i < CONSTANT::NB_JOINTS; i++)
            bodyPos[i] = Point3f();
    }

    void Start()
    {
        println("<Receiver> Starting Receiver");
        if (this->running) return;
        println("<Receiver> Binding socket");
        if (this->socket.bind(0, "localhost") != sf::Socket::Done) return;
        println("<Receiver> Getting informations");
        this->port = this->socket.getLocalPort();
        this->ip = sf::IpAddress::getLocalAddress().toString();
        this->socket.setBlocking(false);

        // connect to fullbowody app
        println("<Receiver> Sending login message");
        this->socket.send(this->LOGIN_MSG.c_str(), this->LOGIN_MSG.size(), "localhost", 5621);

        this->running = true;
        println("<Receiver> Starting thread");
        this->receiver = std::thread(&Receiver::_check_for_data, this);
    }

    void Stop()
    {
        println("<Receiver> Stopping Receiver");
        if (this->running)
        {
            println("<Receiver> Sending logout message");
            this->socket.send(this->LOGOUT_MSG.c_str(), this->LOGOUT_MSG.size(), "localhost", 5621);
            this->running = false;
            println("<Receiver> Joining thread");
            this->receiver.join();
        }
    }

    void _check_for_data()
    {
        while (this->running)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            const unsigned short DATA_LENGTH = 1024;
            char data[DATA_LENGTH];
            std::size_t received;
            sf::IpAddress senderIP;
            unsigned short senderPORT;
            sf::Socket::Status result = this->socket.receive(data, DATA_LENGTH, received, senderIP, senderPORT);
            if (result == sf::Socket::Done)
            {
                println("<Receiver> Received new body data from app");
                unsigned short cursor = 0;
                unsigned short index = 0;
                // get the body points from the string
                while (cursor < DATA_LENGTH && index < CONSTANT::NB_JOINTS)
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
                        bodyPos[index] = Point3f(coordX, coordY, coordZ);
                    } catch (std::exception &e) {break;}
                    index++;
                }
            }
        }
    }

    ~Receiver()
    {
        this->Stop();
    }
};