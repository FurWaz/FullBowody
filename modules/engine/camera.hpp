#pragma once
#include <SFML/System.hpp>
#include <thread>
#include <fstream>
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/aruco.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "../UI/essentials/image.hpp"
#include "./tracker.hpp"

namespace owo
{
    /**
     * @brief CalibrationData class, contains the calibration data of a camera such as
     * the camera matrix and the distance coefficients
     */
    struct CalibrationData
    {
        private:
            std::string CALIBFOLDER = "./resources/calibrations/";
        public:
        cv::Mat cameraMatrix = cv::Mat(3, 3, CV_64F);
        cv::Mat distanceCoefficients = cv::Mat(8, 1, CV_64F);
        bool valid = false;

        /**
         * @brief Loads the calibration data from a given file
         * 
         * @param path The path of the data file
         * @return If the data as been loaded successfully or not
         */
        bool loadFromFile(std::string path)
        {
            bool result = true;
            std::ifstream inStream(CALIBFOLDER+path);
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
            } else result = false;

            inStream.close();
            this->valid = result;
            return result;
        }

        /**
         * @brief Saves the calibration data to a given file
         * 
         * @param path The path of the data file to save
         * @return If the data as been saved successfully or not
         */
        bool saveToFile(std::string path)
        {
            bool result = true;
            std::ofstream outStream(CALIBFOLDER+path);
            if (outStream)
            {
                for (int x = 0; x < this->cameraMatrix.rows; x++)
                {
                    for (int y = 0; y < this->cameraMatrix.cols; y++)
                    {
                        double value = this->cameraMatrix.at<double>(x, y);
                        outStream << value << std::endl;
                    }
                }
                for (int x = 0; x < this->distanceCoefficients.rows; x++)
                {
                    for (int y = 0; y < this->distanceCoefficients.cols; y++)
                    {
                        double value = this->distanceCoefficients.at<double>(x, y);
                        outStream << value << std::endl;
                    }
                }
            } else result = false;

            outStream.close();
            return result;
        }

        /**
         * @brief Returns id the calibration data is valid or not
         * @return a boolean representing is the data is valid
         */
        bool isValid()
        {
            return this->valid;
        }
    };

    /**
     * @brief Camera class, can read a video stream and detect his position from aruco marker board.
     * Contains a tracker object for body position estimation.
     */
    class Camera
    {
    private:
        CalibrationData calibrData;
        cv::VideoCapture source;   // camera's video source path
        Tracker* tracker;          // camera's body position tracker
        cv::Mat frame;             // shown image (with drawings)
        cv::Mat frame_rgb;         // processing image (without drawings)
        cv::Vec3d position;        // position in meters relative to the marker board's origin
        cv::Mat rotation;          // rotation matrix, relative to the markers board's rotation
        cv::Point2d fov;           // half of the camera field of view, in radians

        sf::Vector2u dimensions;           // dimensions of the camera video stream
        std::vector<Image*> graphImages;   // graphical image associated with the camera
        std::string path;                  // path to the camera stream
        bool sourceAvailable;              // is the source of the camera ready
        bool debugMode;                    // is the camera in debug mode (for visual hints)

        bool shouldRead;                   // should the camera thread read the video stream
        std::thread updateThread;          // video stream reading thread

        cv::Ptr<cv::aruco::Dictionary> aruco_dict = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_4X4_50); // aruco dictionnary (aruco x4 models)
        cv::Ptr<cv::aruco::DetectorParameters> aruco_params = cv::aruco::DetectorParameters::create();          // empty parameters for aruco detection
        cv::Ptr<cv::aruco::GridBoard> aruco_board;                                                              // representation of the aruco marker board
        std::vector<std::vector<cv::Point2f>> aruco_corners, aruco_rejected;
        std::vector<int> aruco_ids;
        cv::Vec3d aruco_boardPosition, aruco_boardRotation;

        std::chrono::steady_clock::time_point last_refresh_time;

        /**
         * @brief Set the camera dimensions from source stream
         */
        void setDimensionsFromSource()
        {
            this->source.set(cv::CAP_PROP_BUFFERSIZE, 1);
            this->dimensions = sf::Vector2u(
                this->source.get(cv::CAP_PROP_FRAME_WIDTH),
                this->source.get(cv::CAP_PROP_FRAME_HEIGHT)
            );
        }

        /**
         * @brief Initialises the camera's attributes
         */
        void init()
        {
            this->tracker = new Tracker();
            this->dimensions = sf::Vector2u(300, 300);
            this->frame = cv::Mat(this->dimensions.x, this->dimensions.y, CV_8UC3, cv::Scalar(0, 0, 0));
            this->frame_rgb = cv::Mat(this->dimensions.x, this->dimensions.y, CV_8UC3, cv::Scalar(0, 0, 0));
            this->sourceAvailable = false;
            this->shouldRead = false;
            this->debugMode = false;
            this->rotation = cv::Mat::eye(cv::Size(3, 3), CV_64F);
            this->aruco_board = cv::aruco::GridBoard::create(3, 2, 0.0876, 0.005, this->aruco_dict, 0);

            this->last_refresh_time = std::chrono::steady_clock::now();
        }

        /**
         * @brief Calculates the camera position and rotation from aruco marker board rotation and position
         */
        void getCamPosRot()
        {
            cv::Mat rotMat(3, 3, CV_64F);
            cv::Rodrigues(this->aruco_boardRotation, rotMat);
            cv::Mat pos(3, 1, CV_64F);
            rotMat = rotMat.t();
            pos = -rotMat * this->aruco_boardPosition;

            this->position[0] = pos.at<double>(0, 0);
            this->position[1] = pos.at<double>(1, 0);
            this->position[2] = pos.at<double>(2, 0);
            this->rotation = rotMat;
        }

        /**
         * @brief Detects the aruco markers in the current frame
         */
        void detectArucoMarkers()
        {
            cv::aruco::detectMarkers(
                this->frame_rgb, this->aruco_dict,
                this->aruco_corners, this->aruco_ids, this->aruco_params, this->aruco_rejected
            );
        }

        /**
         * @brief Calculates the aruco markers board position and rotation from the detected aruco markers
         * 
         */
        void getArucoBoardPosition()
        {
            if (this->calibrData.isValid())
                cv::aruco::estimatePoseBoard(
                    this->aruco_corners, this->aruco_ids, this->aruco_board,
                    this->calibrData.cameraMatrix, this->calibrData.distanceCoefficients,
                    this->aruco_boardRotation, this->aruco_boardPosition, false
                );
        }

        /**
         * @brief Calculates the current FOV of the camera from the camera matrix
         * 
         */
        void calculateFOV()
        {
            double fx = this->calibrData.cameraMatrix.at<double>(0, 0);
            double fy = this->calibrData.cameraMatrix.at<double>(1, 1);
            double x = this->calibrData.cameraMatrix.at<double>(0, 2);
            double y = this->calibrData.cameraMatrix.at<double>(1, 2);

            this->fov = cv::Point2d(std::atan(x / fx), std::atan(y / fy));
        }

        bool loadSource(std::string address)
        {
            this->path = address;
            this->sourceAvailable = false;
            if (this->shouldRead)
            {
                this->shouldRead = false;
                this->updateThread.join();
            }
            bool result = true;
            // check if input is number or not (for device index or url)
            int deviceIndex = -1;
            try
            {
                deviceIndex = std::stoi(address);
            } catch (std::exception &e) {}

            // open device index or url
            try
            {
                result = (deviceIndex<0)? this->source.open(address): this->source.open(deviceIndex);
                this->setDimensionsFromSource();
            }
            catch (std::exception &e)
            {
                std::cerr << "err: " << std::endl << e.what();
            }
            if (!result)
            {
                this->dimensions = sf::Vector2u(300, 300);
                for (Image* im: this->graphImages)
                    im->black(sf::Vector2u(300, 300));
            } else
                this->updateThread = std::thread(&Camera::_read_frame, this);
            return result;
        }

    public:
        Camera()
        {
            this->init();
        }

        Camera(Image* im)
        {
            this->attachImage(im);
            this->init();
        }
        
        /**
         * @brief Opens a video stream
         * 
         * @param index URL of the video stream (mjpeg stream)
         * @return If the source as been opened successfully or not
         */
        bool openSource(std::string address)
        {
            return this->loadSource(address);
        }

        /**
         * @brief Updates the displayed frame on the attached image
         * 
         * @param dt Visual delta time
         */
        void updateFrame(float dt)
        {
            std::chrono::steady_clock::time_point current_refresh_time = std::chrono::steady_clock::now();
            if ( std::chrono::duration_cast<std::chrono::milliseconds>( current_refresh_time-last_refresh_time ).count() < CONSTANT::CAMERA_FPS_DELTA )
                return;
            this->last_refresh_time = current_refresh_time;

            this->sourceAvailable =
                this->frame.cols == this->dimensions.x &&
                this->frame.rows == this->dimensions.y;

            if (this->graphImages.size() < 1 || !this->shouldRead || !this->sourceAvailable)
                return;
            try
            {
                if (this->debugMode)
                {
                    detectArucoMarkers();
                    getArucoBoardPosition();
                    getCamPosRot();
                    if (this->aruco_ids.size() > 0)
                    {
                        cv::aruco::drawDetectedMarkers(this->frame, this->aruco_corners, this->aruco_ids);
                        std::vector<cv::Vec3d> tvec, rvec;
                        cv::aruco::estimatePoseSingleMarkers(this->aruco_corners, 0.088, this->calibrData.cameraMatrix, this->calibrData.distanceCoefficients, rvec, tvec);
                        for(int i = 0; i < this->aruco_ids.size(); i++)
                        {
                            cv::drawFrameAxes(
                                this->frame, this->calibrData.cameraMatrix, this->calibrData.distanceCoefficients,
                                rvec[i], tvec[i], 0.03, 6
                            );
                        }
                        cv::drawFrameAxes(
                            this->frame, this->calibrData.cameraMatrix, this->calibrData.distanceCoefficients,
                            this->aruco_boardRotation, this->aruco_boardPosition, 0.05, 8
                        );
                    }
                    std::array<cv::Point3f, CONSTANT::NB_JOINTS> tPoints = this->tracker->getPoints();
                    if (tPoints.size() == CONSTANT::NB_JOINTS)
                    {
                        for(int i = 0; i < CONSTANT::NB_CONNECTIONS; i++)
                        {
                            if (tPoints[CONSTANT::POSE_CONNECTIONS[i][0]].z < 0.8f || tPoints[CONSTANT::POSE_CONNECTIONS[i][1]].z < 0.8f)
                                continue;
                            cv::Point p1(this->frame.cols * tPoints[CONSTANT::POSE_CONNECTIONS[i][0]].x, this->frame.rows * tPoints[CONSTANT::POSE_CONNECTIONS[i][0]].y);
                            cv::Point p2(this->frame.cols * tPoints[CONSTANT::POSE_CONNECTIONS[i][1]].x, this->frame.rows * tPoints[CONSTANT::POSE_CONNECTIONS[i][1]].y);
                            cv::line(this->frame, p1, p2, cv::Scalar(0, 0, 255), 4);
                        }
                    }
                }
                std::vector<std::vector<cv::Point2f>> imgPoints(1);
                cv::Mat rgba;
                cv::cvtColor(this->frame, rgba, cv::COLOR_BGR2RGBA);
                for (Image* im: this->graphImages)
                    im->fromArray(rgba.ptr(), this->dimensions);
            }
            catch (std::exception &e) 
            {
                for (Image* im: this->graphImages)
                    im->black(sf::Vector2u(300, 300));
            }
        }

        void calculatePosition()
        {
            detectArucoMarkers();
            getArucoBoardPosition();
            getCamPosRot();
        }

        /**
         * @brief Attaches the given image to the camera preview
         * 
         * @param im The image to attach to the camera
         */
        void attachImage(Image* im)
        {
            this->graphImages.push_back(im);
        }

        /**
         * @brief Detaches the given image from the camera preview
         * 
         * @param im The image to attach to the camera
         */
        void detachImage(Image* im)
        {
            int index = -1;
            int counter = 0;
            for (Image* img: this->graphImages)
            {
                if (img == im)
                {
                    index = counter;
                    break;
                }
                counter++;
            }
            if (index >= 0)
                this->detachImage(index);
        }

        /**
         * @brief Detaches the given image from the camera preview
         * 
         * @param im The image to attach to the camera
         */
        void detachImage(int index)
        {
            this->graphImages.erase(this->graphImages.begin()+index);
        }
        
        /**
         * @brief [INTERNAL] Reads frames from the video device/stream 
         */
        void _read_frame()
        {
            this->shouldRead = true;
            while (this->shouldRead)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
                if(!this->source.grab()) continue;
                this->source.retrieve(this->frame);
                cv::cvtColor(this->frame, this->frame_rgb, cv::COLOR_BGR2RGB);
            }
        }
        
        /**
         * @brief Sends an image to the tracker for body position detection
         */
        void getBodyPosition()
        {
            this->tracker->sendImage(this->frame_rgb);
        }

        /**
         * @brief Returns the camera current video device index or video stream URL
         * @return A string of the url or device index
         */
        std::string getPath()
        {
            return this->path;
        }

        /**
         * @brief Loads the calibration data of the given file
         * 
         * @param path Path to the data file
         */
        void loadCalibration(std::string path)
        {
            this->calibrData.loadFromFile(path);
            this->calculateFOV();
        }

        /**
         * @brief Saves the calibration data to the given file
         * 
         * @param path Path to the data file
         */
        void saveCalibration(std::string path)
        {
            this->calibrData.saveToFile(path);
        }

        /**
         * @brief Returns the position of the camera
         * @return A 3D point corresponding to the camera's position
         */
        cv::Vec3d getPosition()
        {
            return this->position;
        }

        /**
         * @brief Returns the rotation of the camera
         * @return A rotation matrix correponding to the camera's rotation
         */
        cv::Mat getRotation()
        {
            return this->rotation;
        }

        /**
         * @brief Returns the camera's tracker object
         * @return The tracker of the camera
         */
        Tracker* getTracker()
        {
            return this->tracker;
        }

        /**
         * @brief Returns the camera's calibration data object
         * @returns the camera's calibration
         */
        CalibrationData getCalibrationData()
        {
            return this->calibrData;
        }

        /**
         * @brief Returns the current FOV of the camera according to it's calibration data
         * @return A 2D point correponding to the camera's FOV in radians (dividec by 2)
         */
        cv::Point2d getFOV()
        {
            return this->fov;
        }

        /**
         * @brief Get the Raw camera data
         * @return The current raw image matrix of the camera
         */
        cv::Mat getRawImage()
        {
            return this->frame;
        }

        /**
         * @brief Returns if the camera is in debug mode
         * @return If the camera is in debug mode or not
         */
        bool isDebugMode()
        {
            return this->debugMode;
        }

        /**
         * @brief Sets the camera's debug mode to the given state
         * @param state The new state of the camera's debug mode
         */
        void setDebugMode(bool state)
        {
            this->debugMode = state;
        }

        ~Camera()
        {
            if (this->shouldRead)
            {
                this->shouldRead = false;
                this->updateThread.join();
            }
        }
    };
}