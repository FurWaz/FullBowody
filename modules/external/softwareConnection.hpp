#pragma once
#include <SFML/Network.hpp>
#include <opencv2/core.hpp>
#include <thread>
#include "../constants.hpp"

typedef struct SoftwareInfo {
    unsigned short port;
    std::string name;
    SoftwareInfo(): port(0), name("") {}
    SoftwareInfo(unsigned short p, std::string n): port(p), name(n) {}
} SoftwareInfo;

std::string to_string(sf::Socket::Status st)
{
    if (st == sf::Socket::Done) return "Done";
    if (st == sf::Socket::NotReady) return "NotReady";
    if (st == sf::Socket::Partial) return "Partial";
    if (st == sf::Socket::Disconnected) return "Disconnected";
    if (st == sf::Socket::Error) return "Error";
    return "";
}

class SoftwareConnection
{
private:
    sf::UdpSocket socket;
    bool ready;
    bool newDataAvailable;
    std::vector<SoftwareInfo> appsPorts;

    cv::Vec3d* bodyPosition;

    std::thread listenerThread;
    std::thread senderThread;

    bool setupConnection()
    {
        if (socket.bind(5621, "localhost") != sf::Socket::Done)
        {
            std::cerr << "Error: Cannot bind socket port 5621, aborting" << std::endl;
            return false;
        }
        socket.setBlocking(false);
        return true;
    }

    void setupListenerThread()
    {
        this->listenerThread = std::thread(&SoftwareConnection::_read_input_packets, this);
    }

    void setupSenderThread()
    {
        this->senderThread = std::thread(&SoftwareConnection::_send_body_positions, this);
    }

    std::string toString(double number)
    {
        return std::to_string( ((int)(number * 100))*0.01d );
    }

public:
    SoftwareConnection()
    {
        this->newDataAvailable = false;
    }

    void startConnection()
    {
        this->ready = this->setupConnection();
        if (this->ready)
        {
            this->setupListenerThread();
            this->setupSenderThread();
        }
    }

    void stopConnection()
    {
        if (this->ready)
        {
            this->ready = false;
            this->listenerThread.join();
            this->senderThread.join();
        }
    }

    void _send_body_positions()
    {
        while (this->ready)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            if (!this->newDataAvailable) continue;
            this->newDataAvailable = false;
            std::string data;
            for(int i = 0; i < CONSTANT::NB_JOINTS; i++)
                data += toString(this->bodyPosition[i][0])+"|"+toString(this->bodyPosition[i][1])+"|"+toString(this->bodyPosition[i][2])+"\n";
            const char* string = data.c_str();
            unsigned short length = data.size();

            for (SoftwareInfo soft: this->appsPorts)
            {
                if (this->socket.send(string, length, "localhost", soft.port) == sf::Socket::Done)
                    std::cout << "position sent to port " << soft.port << " (" << soft.name << ")" << std::endl;
                else std::cout << "failed to send body position on port " << soft.port << std::endl;
            }
        }
    }

    void _read_input_packets()
    {
        while (this->ready)
        {
            char data[100];
            std::size_t received;
            sf::IpAddress sender;
            unsigned short port;
            sf::Socket::Status result = this->socket.receive(data, 100, received, sender, port);
            if (result == sf::Socket::Done)
            {
                if (received < 11) continue;
                std::string string(data, received);
                if (string.substr(0, 11) != "FullBowody-") continue;
                std::string appName = string.substr(11, string.size());
                this->appsPorts.push_back(SoftwareInfo(port, appName));
                std::cout << ">> Info: New connection from application [" << appName << "] on port " << port << std::endl;
            }
            else
            {
                if (result == sf::Socket::NotReady) continue;
                std::cerr << "Unknown error: " << to_string(result) << ", closing all apps" << std::endl;
                this->appsPorts.clear();
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }

    void sendNewBodyPosition(cv::Vec3d* bodyPos)
    {
        if (!this->ready) return;
        this->newDataAvailable = true;
        this->bodyPosition = bodyPos;
    }

    ~SoftwareConnection()
    {
        this->stopConnection();
    }
};