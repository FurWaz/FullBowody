#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>
#include <vector>
#include <thread>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

ushort _port_counter = 5621;

namespace owo {
    class Tracker
    {
    private:
        bool pythonStarted;
        sf::UdpSocket socket;
        std::string camAddress;
        std::thread pythonThread;
        std::thread connectionThread;
        ushort pythonPort;
        
    public:
        Tracker(std::string cameraAddress)
        {
            this->pythonPort = _port_counter++;
            this->camAddress = cameraAddress;
            startPython();
        }

        void _start_subscript()
        {
            std::string cmd = "python ./modules/engine/tracker.py ";
            std::string port = std::to_string(pythonPort);
            system( (cmd+port).c_str() );
        }

        void startPython()
        {
            if (pythonStarted) return;
            this->pythonThread = std::thread(&Tracker::_start_subscript, this);
            this->connectionThread = std::thread(&Tracker::_retrieve_positions, this);
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            if (socket.send(this->camAddress.c_str(), this->camAddress.size(), "localhost", pythonPort) != sf::Socket::Done)
                std::cout << "Error sending UDP data" << std::endl;
            else
                std::cout << "Succesfully sent UDP data" << std::endl;
        }

        void stopPython()
        {
            if (socket.send("STOP", 4, "localhost", pythonPort) != sf::Socket::Done)
                std::cout << "Error sending UDP data" << std::endl;
            else
                std::cout << "Succesfully sent UDP data" << std::endl;
            this->pythonThread.join();
            this->connectionThread.join();
            this->pythonStarted = false;
        }

        void _retrieve_positions()
        {
            while (this->pythonStarted)
            {
                char str[400];
                sf::IpAddress sender;
                unsigned short port;
                std::size_t received;
                if (this->socket.receive(str, 400, received, sender, port) != sf::Socket::Done)
                    std::cout << "Error receiving data" << std::endl;
                else
                    std::cout << "Data received: " << str << " on port " << port << std::endl;
                this->getPointsFromData(str, 400);
            }
        }

        void getPointsFromData(char* data, ushort length)
        {
            std::vector<cv::Point2i> points;
            ushort cursor = 0;
            while (cursor < length)
            {
                ushort lastPos = cursor;

                // get x coordonnate
                char nbrX[5];
                ushort index = 0;
                while (data[cursor] != '|')
                    nbrX[index++] = data[cursor++];
                cursor++;
                std::cout << "coord X: " << nbrX << " cursorIndex: " << cursor << std::endl;

                // get y coordonnate
                char nbrY[5];
                index = 0;
                while (data[cursor] != '\n')
                    nbrY[index++] = data[cursor++];
                cursor++;
                std::cout << "coord Y: " << nbrY << " cursorIndex: " << cursor << std::endl;

                // add coordonnates to vector
                int coordX = std::stoi(nbrX);
                int coordY = std::stoi(nbrY);
                points.push_back( cv::Point2i(coordX, coordY) );
                std::cout << "Added point: " << coordX << ", " << coordY << std::endl;
            }
        }

        void startTracking()
        {
            this->startPython();
        }

        void stopTracking()
        {
            this->stopPython();
        }

        ~Tracker()
        {
            if (this->pythonStarted)
            {
                this->pythonThread.join();
                this->connectionThread.join();
            }
        }
    };
}