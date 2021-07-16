#pragma once
#include <opencv2/core.hpp>
#include <opencv2/aruco.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "../UI/image.hpp"
#include "./tracker.hpp"
#include <SFML/System.hpp>
#include <thread>
#include <fstream>
#include <iostream>

namespace owo
{
    struct CalibrationData
    {
        cv::Mat cameraMatrix = cv::Mat(3, 3, CV_64F);
        cv::Mat distanceCoefficients = cv::Mat(8, 1, CV_64F);

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
        cv::Mat frame_gray;
        cv::Vec3d rotation, position;
        Tracker* tracker;

        sf::Vector2u dimensions;
        Image* graphImage;
        std::string path;
        bool sourceAvailable;
        bool debugMode;

        bool shouldRead;
        std::thread updateThread;
        float delta;
        float FPS;

        cv::Ptr<cv::aruco::Dictionary> aruco_dict = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_4X4_50);
        cv::Ptr<cv::aruco::DetectorParameters> aruco_params = cv::aruco::DetectorParameters::create();
        cv::Ptr<cv::aruco::GridBoard> aruco_board;
        std::vector<std::vector<cv::Point2f>> aruco_corners, aruco_rejected;
        std::vector<int> aruco_ids;
        cv::Vec3d aruco_boardPosition, aruco_boardRotation;

        void setDimensionsFromSource()
        {
            this->dimensions = sf::Vector2u(
                this->source.get(cv::CAP_PROP_FRAME_WIDTH),
                this->source.get(cv::CAP_PROP_FRAME_HEIGHT)
            );
            this->sourceAvailable = true;
        }

        void init()
        {
            this->tracker = new Tracker(this->path);
            this->dimensions = sf::Vector2u(300, 300);
            this->frame = cv::Mat(this->dimensions.x, this->dimensions.y, CV_8UC3, cv::Scalar(0, 0, 0));
            this->frame_gray = cv::Mat(this->dimensions.x, this->dimensions.y, CV_8U);
            this->sourceAvailable = false;
            this->shouldRead = false;
            this->debugMode = false;
            this->delta = 0.f;
            this->FPS = 30.f;
            this->aruco_board = cv::aruco::GridBoard::create(3, 2, 0.088, 0.005, this->aruco_dict, 0);
        }

        void getCamPosRot()
        {
            cv::Mat rotMat(3, 3, CV_64F);
            cv::Rodrigues(this->aruco_boardRotation, rotMat);
            rotMat = rotMat.t();
            cv::Rodrigues(rotMat, this->rotation);
            cv::Mat position(3, 1, CV_64F);
            position = -rotMat * this->aruco_boardPosition;
            this->position[0] = position.at<double>(0, 0);
            this->position[1] = position.at<double>(1, 0);
            this->position[2] = position.at<double>(2, 0);
        }

        void detectArucoMarkers()
        {
            cv::aruco::detectMarkers(
                this->frame_gray, this->aruco_dict,
                this->aruco_corners, this->aruco_ids, this->aruco_params, this->aruco_rejected
            );
        }

        void getArucoBoardPosition()
        {
            cv::aruco::estimatePoseBoard(
                this->aruco_corners, this->aruco_ids, this->aruco_board,
                this->calibrData.cameraMatrix, this->calibrData.distanceCoefficients,
                this->aruco_boardRotation, this->aruco_boardPosition, false
            );
            getCamPosRot();
        }

    public:
        Camera()
        {
            this->init();
        }

        Camera(Image* im)
        {
            this->graphImage = im;
            this->init();
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

        void updateFrame(float dt)
        {
            std::cout << "-> update frame" << std::endl;
            if (this->graphImage == nullptr) return;
            try
            {
                this->source.retrieve(this->frame);
                if (this->debugMode)
                {
                    cv::aruco::drawDetectedMarkers(this->frame, this->aruco_corners, this->aruco_ids);
                    cv::drawFrameAxes(
                        this->frame, this->calibrData.cameraMatrix, this->calibrData.distanceCoefficients,
                        this->aruco_boardRotation, this->aruco_boardPosition, 0.042, 6
                    );
                }
                cv::Mat rgba;
                cv::cvtColor(this->frame, rgba, cv::COLOR_BGR2RGBA);
                this->graphImage->fromArray(rgba.ptr(), this->dimensions);
            }
            catch (std::exception &e) 
            {
                std::cerr << e.what() << std::endl;
                this->graphImage->black(sf::Vector2u(300, 300));
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
                if (!this->source.grab()) continue;
                this->source.retrieve(this->frame);
                cv::cvtColor(this->frame, this->frame_gray, cv::COLOR_BGR2GRAY);
                detectArucoMarkers();
                getArucoBoardPosition();
                std::cout << "<- retreive frame" << std::endl;
            }
            this->shouldRead = false;
        }

        std::string getPath()
        {
            return this->path;
        }

        void loadCalibration(std::string path)
        {
            this->calibrData.loadFromFile(path);
        }

        void saveCalibration(std::string path)
        {
            this->calibrData.saveToFile(path);
        }

        cv::Vec3d getPosition()
        {
            return this->position;
        }

        cv::Vec3d getRotation()
        {
            return this->rotation;
        }

        Tracker* getTracker()
        {
            return this->tracker;
        }

        void setDebugMode(bool state)
        {
            this->debugMode = state;
        }

        ~Camera()
        {
            this->shouldRead = false;
            this->updateThread.join();
        }
    };
}