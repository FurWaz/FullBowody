#pragma once
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
#include "../constants.hpp"
#include "../UI/essentials/callbackContainer.hpp"
#include "./ipc.hpp"

/**
 * @brief Returns the number to divide a float by to get a number between 0 and 1
 * 
 * @param nbr The target number
 * @return The number to divide by
 */
float nbDigitDivide(int nbr)
{
    int i = 1;
    while ( !(nbr < i) )
    {
        i *= 10;
    }
    return i;
}

/**
 * @brief Returns a text representation of the given points
 * 
 * @param pts The Points to display
 * @return A string representation of the given Points list
 */
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

namespace owo
{
    /**
     * @brief Tracker class, uses the IPC class to get the body position of a given frame
     */
    class Tracker
    {
    private:
        IPC ipc;
        bool dataAvailable;
        std::thread sendingThread;
        bool running;
        cv::Mat dataToSend;
        std::array<cv::Point3f, CONSTANT::NB_JOINTS> points; // z is visibility
        CallbackContainer* cont;
        bool newTrackingDataAvailable;

        /**
         * @brief Parses the raw text data to fill the points list the the actual joints positions
         * 
         * @param data The data to parse
         * @param length The length of the data to parse
         */
        void getPointsFromData(char* data, unsigned short length)
        {
            unsigned short cursor = 0;
            unsigned short index = 0;
            std::array<cv::Point3f, CONSTANT::MEDIAPIPE_JOINTS> joints;
            // get the mediapipe body points from the string
            while (cursor < length && index < CONSTANT::MEDIAPIPE_JOINTS)
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
                    joints[index] = p;
                } catch (std::exception &e) {break;}
                index++;
            }
            // calculates the app's body position from mediapipe
            this->points[CONSTANT::JOINT_NOSE] = joints[0];
            this->points[CONSTANT::JOINT_HEAD] = (joints[8] + joints[7]) / 2;
            this->points[CONSTANT::JOINT_NECK] = (joints[12] + joints[11]) / 2;
            this->points[CONSTANT::JOINT_SHOULDER_R] = joints[12];
            this->points[CONSTANT::JOINT_SHOULDER_L] = joints[11];
            this->points[CONSTANT::JOINT_ELBOW_R] = joints[14];
            this->points[CONSTANT::JOINT_ELBOW_L] = joints[13];
            this->points[CONSTANT::JOINT_WRIST_R] = joints[16];
            this->points[CONSTANT::JOINT_WRIST_L] = joints[15];
            this->points[CONSTANT::JOINT_HAND_R] = (joints[20] + joints[18]) / 2;
            this->points[CONSTANT::JOINT_HAND_L] = (joints[19] + joints[17]) / 2;
            this->points[CONSTANT::JOINT_HIP_R] = joints[24];
            this->points[CONSTANT::JOINT_HIP_L] = joints[23];
            this->points[CONSTANT::JOINT_KNEE_R] = joints[26];
            this->points[CONSTANT::JOINT_KNEE_L] = joints[25];
            this->points[CONSTANT::JOINT_ANKLE_R] = cv::Point3f(joints[28].x, joints[32].y, joints[28].z);
            this->points[CONSTANT::JOINT_ANKLE_L] = cv::Point3f(joints[27].x, joints[31].y, joints[27].z);
            this->points[CONSTANT::JOINT_FEET_R] = joints[32];
            this->points[CONSTANT::JOINT_FEET_L] = joints[31];
        }
        
    public:
        /**
         * @brief Default constructor
         */
        Tracker()
        {
            this->newTrackingDataAvailable = false;
            this->dataAvailable = false;
            this->ipc.setReadCallback(&Tracker::_retrieve_positions, this);
        }

        /**
         * @brief [INTERNAL] Checks for a new image from the camera to process
         */
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
                    std::this_thread::sleep_for(std::chrono::milliseconds(2));
                } else
                {
                    std::this_thread::sleep_for(std::chrono::milliseconds(10));
                }
            }
        }

        /**
         * @brief [INTERNAL] Retrieves the positions of the body joints.
         * Used as callback in the IPC
         * @param data The data sent from the child process
         * @param length The length of the data
         */
        void _retrieve_positions(char* data, unsigned short length)
        {
            std::string str(data, length);
            if (this->dataAvailable)
            {
                getPointsFromData(data, length);
                this->setNewTrackingDataAvailable(true);
                this->cont->func();
            }

            if (str == "READY")
                this->dataAvailable = true;
        }

        /**
         * @brief Sends the given image to the child process
         * for body position detection
         * @param img The image to send to the child process
         */
        void sendImage(cv::Mat img)
        {
            if (!this->running) return;
            this->dataToSend = img;
        }

        /**
         * @brief Starts the IPC and the tracker's threads to track the body position
         * from the given images
         */
        void startTracking()
        {
            this->ipc.startChild();
            this->sendingThread = std::thread(&Tracker::_check_for_input_data, this);
            this->running = true;
        }

        /**
         * @brief Stops the tracker's threads and the IPC connection
         */
        void stopTracking()
        {
            if (!this->running) return;
            this->dataAvailable = false;
            this->ipc.stopChild();
            this->running = false;
            this->sendingThread.join();
        }

        /**
         * @brief Sets the callback for new data recieved event
         * 
         * @tparam T The callback class
         * @param callback The callback method
         * @param c The callback medhod's class instance
         */
        template<class T> void setNewDataCallback(void (T::*callback)(), T* c)
        {
            this->cont = new TypedCallbackContainer<T>(callback, c);
        }

        /**
         * @brief Returns the 2D points of the detected body position (between 0 and 1)
         * @return a 2D point list corresponding to the positions of the body joints (z axis is the visibility of the point)
         */
        std::array<cv::Point3f, CONSTANT::NB_JOINTS> getPoints()
        {
            return this->points;
        }

        /**
         * @brief Returns if new tracking data is available
         * @return Boolean if the data is available or not
         */
        bool isNewTrackingDataAvailable()
        {
            return this->newTrackingDataAvailable;
        }

        /**
         * @brief Sets if new tracking data is available or not
         * @param state The new value
         */
        void setNewTrackingDataAvailable(bool state)
        {
            this->newTrackingDataAvailable = state;
        }

        ~Tracker()
        {
            this->stopTracking();
        }
    };
}