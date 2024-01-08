#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>
#include <opencv2/aruco.hpp>
#include <opencv2/calib3d.hpp>
#include <SFML/Graphics.hpp>
#include <fstream>

std::vector<cv::Mat> samples;
cv::Size boardSize(8, 5);
float squareSize = 2.8;

std::vector<cv::Mat> rVectors, tVectors;
cv::Mat distCoeffs(8, 1, CV_64F);
cv::Mat camMatrix(3, 3, CV_64F);

bool captureSamples()
{
    sf::RenderWindow win(sf::VideoMode(720, 1280), "Samples capture", sf::Style::Default);
    sf::Event ev;

    cv::VideoCapture cap;
    cap.open("http://192.168.0.3:8080/video");
    if (!cap.isOpened())
    {
        std::cout << "Error reading stream" << std::endl;
        std::cout << "i" << std::endl;
        return false;
    }
    int i = 0;
    while (win.isOpen())
    {
        win.pollEvent(ev);
        cv::Mat bgr;
        cv::Mat gray;
        cv::Mat rgba;
        cap.read(bgr);
        cv::cvtColor(bgr, gray, cv::COLOR_BGR2GRAY);
        cv::cvtColor(bgr, rgba, cv::COLOR_BGR2RGBA);
        
        std::vector<cv::Point2f> imgPoints;

        sf::Image im;
        im.create(rgba.cols, rgba.rows, rgba.ptr());
        sf::Texture tex;
        tex.loadFromImage(im);
        
        sf::Sprite spr(tex);
        win.draw(spr);
        bool result = false;
        if (i < 1)
        {
            result = cv::findChessboardCorners(bgr, boardSize, imgPoints, cv::CALIB_CB_ADAPTIVE_THRESH | cv::CALIB_CB_NORMALIZE_IMAGE);
            if (result) {
                for (int i = 0; i < imgPoints.size()-1; i++)
                {
                    sf::Vertex line[2] = {
                        sf::Vertex(sf::Vector2f(imgPoints[i].x, imgPoints[i].y), sf::Color::Red, sf::Vector2f()),
                        sf::Vertex(sf::Vector2f(imgPoints[i+1].x, imgPoints[i+1].y), sf::Color::Blue, sf::Vector2f())
                    };
                    win.draw(line, 2, sf::Lines);
                }
            } else
            {
                i += 10;
            }
        } else
            i--;

        win.display();
        
        if (ev.type == sf::Event::KeyPressed)
        {
            if (ev.key.code == sf::Keyboard::Space)
            {
                if (!result)
                {
                    std::cout << "No chess trackers found - canceling save" << std::endl;
                } else
                {
                    cv::Mat temp;
                    gray.copyTo(temp);
                    samples.push_back( temp );
                    std::cout << "Captured image " << samples.size() << std::endl;
                }
            }
            if (ev.key.code == sf::Keyboard::Enter)
            {
                win.close();
                std::cout << "e" << std::endl;
                return true;
            }
        }
    }
    std::cout << "y" << std::endl;
    return false;
}

void calibrateCam()
{
    std::vector<std::vector<cv::Point2f>> imgPoints(samples.size());
    std::vector<std::vector<cv::Point3f>> objPoints(1);
    // createKnownBoardPosition
    for (int i = 0; i < boardSize.height; i++)
        for (int j = 0; j < boardSize.width; j++)
            objPoints[0].push_back(cv::Point3f(j * squareSize, i * squareSize, 0.f));    
    objPoints.resize(imgPoints.size(), objPoints[0]);

    for (int s = 0; s < samples.size(); s++)
    {
        cv::Mat gray = samples[s];
        bool result = cv::findChessboardCorners(gray, boardSize, imgPoints[s], cv::CALIB_CB_ADAPTIVE_THRESH | cv::CALIB_CB_NORMALIZE_IMAGE);
        if (!result) continue;

        // calibrate
        cv::cornerSubPix(gray, imgPoints[s], cv::Size(10, 10), cv::Size(-1, -1), cv::TermCriteria(cv::TermCriteria::EPS+cv::TermCriteria::COUNT, 30, 0.1));

    }
    cv::calibrateCamera(objPoints, imgPoints, boardSize, camMatrix, distCoeffs, rVectors, tVectors);
    
}

void saveInfos()
{
    std::ofstream outStream("note9_lite.txt");
    if (outStream)
    {
        uint16_t rows = camMatrix.rows;
        uint16_t cols = camMatrix.cols;

        for (int x = 0; x < rows; x++)
        {
            for (int y = 0; y < cols; y++)
            {
                double value = camMatrix.at<double>(x, y);
                outStream << value << std::endl;
            }
            
        }

        rows = distCoeffs.rows;
        cols = distCoeffs.cols;
        
        for (int x = 0; x < rows; x++)
        {
            for (int y = 0; y < cols; y++)
            {
                double value = distCoeffs.at<double>(x, y);
                outStream << value << std::endl;
            }
            
        }
    }
    outStream.close();
}

int main(int argc, char const *argv[])
{
    std::cout << "getting samples ..." << std::endl;
    bool res = captureSamples();

    if (!res)
    {
        std::cout << "captureSamples returned false, aborting" << std::endl;
        return 1;
    }
    if (samples.size() < 15)
    {
        std::cout << "not enough data, aborting" << std::endl;
        return 1;
    }

    std::cout << "calibrating ..." << std::endl;
    calibrateCam();
    
    std::cout << "saving ..." << std::endl;
    saveInfos();
    return 0;
}