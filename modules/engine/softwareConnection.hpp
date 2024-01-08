#pragma once
#include <SFML/Network.hpp>
#include <opencv2/core.hpp>
#include <thread>
#include "../constants.hpp"
#include "../UI/essentials/callbackContainer.hpp"

namespace owo
{
    typedef struct SoftwareInfo {
        unsigned short port;
        std::string ip;
        std::string name;
        SoftwareInfo(): port(0), ip("localhost"), name("") {}
        SoftwareInfo(unsigned short p, std::string n, std::string i = "localhost"): port(p), ip(i), name(n) {}
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
        unsigned short port;
        std::string ip;
        bool ready;
        bool newDataAvailable;
        std::vector<SoftwareInfo> extensions;

        std::array<cv::Vec3d, CONSTANT::NB_JOINTS> bodyPosition;

        std::thread listenerThread;
        std::thread senderThread;

        CallbackContainer* cont;

        bool setupConnection()
        {
            this->ip = sf::IpAddress::getLocalAddress().toString();
            this->port = 5621;
            while (socket.bind(this->port, "localhost") != sf::Socket::Done)
                this->port++;
            std::cout << ">> Info: Extensions port is " << this->port << " on IP address " << this->ip << std::endl;
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
            return std::to_string(number);
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

                for (SoftwareInfo soft: this->extensions)
                {
                    if (this->socket.send(string, length, soft.ip, soft.port) != sf::Socket::Done)
                        ;// debug
                }
            }
        }

        void _read_input_packets()
        {
            while (this->ready)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(200));
                char data[100];
                std::size_t received;
                sf::IpAddress softIP;
                unsigned short softPORT;
                sf::Socket::Status result = this->socket.receive(data, 100, received, softIP, softPORT);
                if (result == sf::Socket::Done)
                {
                    if (received < 15) continue;
                    std::string string(data, received);
                    if (string.substr(0, 15) == "[ON]FullBowody-")
                    {
                        std::string appName = string.substr(15, string.size());
                        this->extensions.push_back(SoftwareInfo(softPORT, appName, softIP.toString()));
                    } else if (string.substr(0, 16) == "[OFF]FullBowody-")
                    {
                        int index = -1;
                        int counter = 0;
                        for (SoftwareInfo soft: this->extensions)
                        {
                            if (soft.port == softPORT && soft.ip == softIP)
                            {
                                index = counter;
                                break;
                            }
                            counter++;
                        }
                        if (index >= 0)
                            this->extensions.erase(this->extensions.begin()+index);
                    }
                    if (this->cont != nullptr)
                        this->cont->func();
                }
                else
                {
                    if (result == sf::Socket::NotReady) continue;
                    if (result == 3) 
                    {
                        this->extensions.clear();
                        if (this->cont != nullptr)
                            this->cont->func();
                    }
                }
            }
        }

        void sendNewBodyPosition(std::array<cv::Vec3d, CONSTANT::NB_JOINTS> bodyPos)
        {
            if (!this->ready) return;
            this->newDataAvailable = true;
            this->bodyPosition = bodyPos;
        }

        void setExtensionCallback(void(*callback)())
        {
            this->cont = new VoidCallbackContainer(callback);
        }

        std::vector<SoftwareInfo> getExtensions()
        {
            return this->extensions;
        }

        std::string getIP()
        {
            return this->ip;
        }

        unsigned short getPort()
        {
            return this->port;
        }

        ~SoftwareConnection()
        {
            this->stopConnection();
        }
    };
}