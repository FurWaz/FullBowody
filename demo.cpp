#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>
#include <opencv2/aruco.hpp>
#include <opencv2/calib3d.hpp>
#include <SFML/Graphics.hpp>
#include <fstream>

void loadInfos(cv::Mat distCoeffs, cv::Mat camMatrix)
{
    std::ifstream inStream("out.txt");
    if (inStream)
    {
        uint16_t rows = camMatrix.rows;
        uint16_t cols = camMatrix.cols;

        for (int x = 0; x < rows; x++)
        {
            for (int y = 0; y < cols; y++)
            {
                double value;
                inStream >> value;
                camMatrix.at<double>(x, y) = value;
            }
            
        }

        rows = distCoeffs.rows;
        cols = distCoeffs.cols;
        
        for (int x = 0; x < rows; x++)
        {
            for (int y = 0; y < cols; y++)
            {
                double value;
                inStream >> value;
                distCoeffs.at<double>(x, y) = value;
            }
            
        }
    }
    inStream.close();
}

void camPosRot(cv::Mat camPos, cv::Mat camRot, cv::Vec3d rvec, cv::Vec3d tvec)
{
    cv::Mat rotMat(3, 3, CV_64F);
    cv::Rodrigues(rvec, rotMat);
    rotMat = rotMat.t();
    cv::Rodrigues(rotMat, camRot);
    camPos = -rotMat * tvec;
}

int main(int argc, char const *argv[])
{
    sf::RenderWindow win(sf::VideoMode(720, 1280), "Hello", sf::Style::Default);
    sf::Event ev;

    cv::VideoCapture cap;
    cap.open("http://192.168.0.50:8080/video");
    if (!cap.isOpened())
    {
        std::cout << "Error reading stream" << std::endl;
        return 1;
    }

    cv::Ptr<cv::aruco::Dictionary> dict = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_4X4_50);
    cv::Ptr<cv::aruco::DetectorParameters> params = cv::aruco::DetectorParameters::create();
    cv::Ptr<cv::aruco::GridBoard> board = cv::aruco::GridBoard::create(3, 2, 0.088, 0.005, dict, 0);

    sf::Vector2f points[4];
    
    cv::Mat distCoeffs(8, 1, CV_64F);
    cv::Mat camMatrix(3, 3, CV_64F);
    loadInfos(distCoeffs, camMatrix);
    
    float radToDeg = 180/3.1415926f;

    while (win.isOpen())
    {
        win.pollEvent(ev);
        if (ev.type == sf::Event::KeyPressed)
            win.close(); 
        cv::Mat bgr;
        cap.read(bgr);
        std::vector<int> IDs;
        std::vector<std::vector<cv::Point2f>> corners, rejected;
        cv::Mat gray;
        cv::cvtColor(bgr, gray, cv::COLOR_BGR2GRAY);
        cv::aruco::detectMarkers(gray, dict, corners, IDs, params, rejected);
        cv::aruco::drawDetectedMarkers(bgr, corners, IDs);

        std::vector<cv::Vec3d> rvec, tvec;
        cv::aruco::estimatePoseSingleMarkers(corners, 0.084f, camMatrix, distCoeffs, rvec, tvec);

        for (int i = 0; i < IDs.size(); i++)
            cv::drawFrameAxes(bgr, camMatrix, distCoeffs, rvec[i], tvec[i], 0.021, 2); // draw marker axis from camera view

        cv::Vec3d boardPos, boardRot;
        cv::aruco::estimatePoseBoard(corners, IDs, board, camMatrix, distCoeffs, boardRot, boardPos, false);
        cv::drawFrameAxes(bgr, camMatrix, distCoeffs, boardRot, boardPos, 0.042, 4);

        cv::Mat camRot(3, 1, CV_64F), camPos(3, 1, CV_64F);
        camPosRot(camPos, camRot, boardRot, boardPos);

        cv::Mat black(cv::Size(720, 1280), CV_8UC3, cv::Scalar(255, 255, 255));
        cv::drawFrameAxes(black, camMatrix, distCoeffs, camRot, camPos, 0.1, 6);

        cv::Mat rgba;
        cv::cvtColor(black, rgba, cv::COLOR_BGR2RGBA);
        sf::Image im;
        im.create(rgba.cols, rgba.rows, rgba.ptr());
        sf::Texture tex;
        tex.loadFromImage(im);
        
        sf::Sprite spr(tex);
        win.draw(spr);
        win.display();
    }

    return 0;
}