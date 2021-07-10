#pragma once
#include <opencv2/core.hpp>
#include <opencv2/aruco.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "../UI/image.hpp"
#include <SFML/System.hpp>
#include <thread>
#include <fstream>
#include <iostream>

namespace owo {
    struct CalibrationData
    {
        cv::Mat cameraMatrix = cv::Mat(8, 1, CV_64F);
        cv::Mat distanceCoefficients = cv::Mat(3, 3, CV_64F);

        bool loadFromFile(std::string path)
        {
            bool result = true;
            std::ifstream inStream(path);
            if (inStream)
            {
                for (int x = 0; x < this->cameraMatrix.rows; x++)
                {
                    for (int y = 0; y < this->cameraMatrix.cols; y++)
                    {
                        double value;
                        inStream >> value;
                        this->cameraMatrix.at<double>(x, y) = value;
                    }
                }
                for (int x = 0; x < this->distanceCoefficients.rows; x++)
                {
                    for (int y = 0; y < this->distanceCoefficients.cols; y++)
                    {
                        double value;
                        inStream >> value;
                        this->distanceCoefficients.at<double>(x, y) = value;
                    }
                }
            } else 
            {
                std::cerr << "Error opening camera calibration data at " << path << std::endl;
                result = false;
            }
            inStream.close();
            return result;
        }

        bool saveToFile(std::string path)
        {
            bool result = true;
            std::ofstream outStream(path);
            if (outStream)
            {
                for (int x = 0; x < this->cameraMatrix.rows; x++)
                {
                    for (int y = 0; y < this->cameraMatrix.cols; y++)
                    {
                        double value = this->cameraMatrix.at<double>(x, y);
                        outStream << value;
                    }
                }
                for (int x = 0; x < this->distanceCoefficients.rows; x++)
                {
                    for (int y = 0; y < this->distanceCoefficients.cols; y++)
                    {
                        double value = this->distanceCoefficients.at<double>(x, y);
                        outStream << value;
                    }
                }
            } else 
            {
                std::cerr << "Error writing camera calibration data at " << path << std::endl;
                result = false;
            }
            outStream.close();
            return result;
        }
    };


    class Camera
    {
    private:
        CalibrationData calibrData;
        cv::VideoCapture source;
        cv::Mat frame;
        sf::Vector2u dimensions;
        Image* graphImage;
        std::string path;
        bool sourceAvailable;

        bool shouldRead;
        std::thread updateThread;

        void setDimensionsFromSource()
        {
            this->dimensions = sf::Vector2u(
                this->source.get(cv::CAP_PROP_FRAME_WIDTH),
                this->source.get(cv::CAP_PROP_FRAME_HEIGHT)
            );
            this->sourceAvailable = true;
        }

        void initFrame()
        {
            this->frame = cv::Mat(this->dimensions.x, this->dimensions.y, CV_8U);
        }

    public:
        Camera()
        {
            this->dimensions = sf::Vector2u(1280, 720);
            this->initFrame();
            this->sourceAvailable = false;
            this->shouldRead = false;
        }

        Camera(Image* im)
        {
            this->graphImage = im;
            this->sourceAvailable = false;
            this->shouldRead = false;
        }
        
        bool openSource(int index)
        {
            char* str;
            itoa(index, str, 0);
            this->path = str;
            bool result = true;
            try
            {
                result = this->source.open(index);
                this->setDimensionsFromSource();
            }
            catch (std::exception &e)
            {
                std::cerr << "err: " << std::endl << e.what();
            }
            if (this->shouldRead == false)
                this->updateThread = std::thread(&Camera::_read_frame, this);
            if (!result)
            {
                this->dimensions = sf::Vector2u(300, 300);
                this->sourceAvailable = false;
                this->graphImage->black(sf::Vector2u(300, 300));
            }
            return result;
        }
        
        bool openSource(std::string address)
        {
            this->path = address;
            bool result = true;
            try
            {
                result = this->source.open(address);
                this->setDimensionsFromSource();
            }
            catch (std::exception &e)
            {
                std::cerr << "err: " << std::endl << e.what();
            }
            if (this->shouldRead == false)
                this->updateThread = std::thread(&Camera::_read_frame, this);
            if (!result)
            {
                this->dimensions = sf::Vector2u(300, 300);
                this->sourceAvailable = false;
                this->graphImage->black(sf::Vector2u(300, 300));
            }
            return result;
        }

        void updateFrame()
        {
            if (this->graphImage != nullptr)
            {
                try
                {
                    this->source.retrieve(this->frame);
                    cv::Mat rgba;
                    cv::cvtColor(this->frame, rgba, cv::COLOR_BGR2RGBA);
                    this->graphImage->fromArray(rgba.ptr(), this->dimensions);
                }
                catch (std::exception &e) 
                {
                    this->graphImage->black(sf::Vector2u(300, 300));
                }
            }
        }

        void attachImage(Image* im)
        {
            this->graphImage = im;
        }
        
        void _read_frame()
        {
            this->shouldRead = true;
            while (this->shouldRead)
            {
                this->source.grab();
                this->source.retrieve(this->frame);
            }
        }

        std::string getPath()
        {
            return this->path;
        }

        ~Camera()
        {
            this->shouldRead = false;
            this->updateThread.join();
        }
    };
}