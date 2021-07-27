#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <thread>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include "./ipc.hpp"

unsigned char POSE_CONNECTIONS[35][2] = {
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

float nbDigitDivide(int nbr)
{
    int i = 1;
    while ( !(nbr < i) )
    {
        i *= 10;
    }
    return i;
}

std::string displayPoints(std::vector<cv::Point3f> pts)
{
    std::string res = ">> number of points: ";
    res += std::to_string(pts.size()) + "\n";
    for(cv::Point3f p: pts)
    {
        res += "    <";
        res += std::to_string(p.x);
        res += ", ";
        res += std::to_string(p.y);
        res += " - ";
        res += std::to_string((int)std::round(p.z * 100)) + "%";
        res += ">\n";
    }
    return res;
}

namespace owo {
    class Tracker
    {
    private:
        std::string camAddress;
        IPC ipc;
        int FPS = 20;
        bool dataAvailable;
        std::thread sendingThread;
        bool running;
        cv::Mat dataToSend;
        std::vector<cv::Point3f>* points; // z is visibility
        
    public:
        Tracker(std::string cameraAddress)
        {
            this->camAddress = cameraAddress;
            dataAvailable = false;
            this->points = new std::vector<cv::Point3f>();
        }

        void _check_for_input_data()
        {
            while (this->running)
            {
                if (!this->dataToSend.empty())
                {
                    std::vector<unsigned char> buffer;
                    cv::imencode(".png", this->dataToSend, buffer);
                    unsigned char* a = new unsigned char[buffer.size()];
                    std::copy(buffer.begin(), buffer.end(), a);
                    std::string str = std::to_string(buffer.size()); str += "\n";
                    this->ipc.writeToChild(str.c_str(), str.size());
                    this->ipc.writeToChild(a, buffer.size());
                    delete[] a;
                    this->dataToSend = cv::Mat();
                } else
                {
                    std::this_thread::sleep_for(std::chrono::milliseconds(2));
                }
            }
        }

        void _retrieve_positions(char* data, unsigned short length)
        {
            std::string str(data, length);

            if (!this->dataAvailable)
                std::cout << ">>> New input from Python: \n" << str << std::endl;
            else
                getPointsFromData(data, length);

            if (str == "READY")
                this->dataAvailable = true;
        }

        void sendImage(cv::Mat img)
        {
            if (!this->running) return;
            this->dataToSend = img;
        }

        void getPointsFromData(char* data, unsigned short length)
        {
            this->points->clear();
            ushort cursor = 0;
            while (cursor < length && this->points->size() < 33)
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

                    // get visibility
                    std::string vs = "";
                    while (data[cursor] != '\n')
                        vs += data[cursor++];
                    cursor++;

                    // add coordonnates to vector
                    int coordX = std::stoi(nbrX.c_str());
                    int coordY = std::stoi(nbrY.c_str());
                    int visibility = std::stoi(vs.c_str());
                    cv::Point3f p(
                        coordX / nbDigitDivide(coordX), 
                        coordY / nbDigitDivide(coordY), 
                        visibility / nbDigitDivide(visibility)
                    );
                    this->points->push_back( p );
                } catch (std::exception &e) {break;}
            }
        }

        void startTracking()
        {
            this->ipc.setReadCallback(&Tracker::_retrieve_positions, this);
            this->ipc.startChild();
            this->running = true;
            this->sendingThread = std::thread(&Tracker::_check_for_input_data, this);
        }

        void stopTracking()
        {
            this->ipc.stopChild();
            this->running = false;
            this->sendingThread.join();
        }

        void setCameraAddress(std::string address)
        {
            this->camAddress = address;
        }

        std::vector<cv::Point3f> getPoints()
        {
            return *this->points;
        }

        ~Tracker()
        {
            
        }
    };
}