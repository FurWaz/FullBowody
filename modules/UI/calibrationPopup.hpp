#pragma once
#include "../constants.hpp"
#include "./essentials/graphicElement.hpp"
#include "./essentials/button.hpp"
#include "./essentials/image.hpp"
#include "./essentials/list.hpp"
#include "./popup.hpp"
#include "./imagePreview.hpp"
#include "./loadingFrame.hpp"

#include "../engine/camera.hpp"

namespace owo
{
    class CalibrationPopup : public virtual GraphicElement
    {
    private:
        Popup* popup;
        Camera* cam;
        Image* preview;
        List* list;
        Button* addBtn;
        Button* remBtn;
        Button* saveBtn;
        Label* nbImages;
        sf::RenderTexture temp;

        LoadingFrame* lf;
        std::vector<std::unique_ptr<cv::Mat>> images;
        std::vector<cv::Point2f> markers;
        CalibrationData calibData;

        std::thread calibrator;
        bool calibrating;
        bool shouldJoin;
        bool loadPopupDisplayed;
        bool shouldChangeText;
        bool shouldDetectChess;
        std::string newText;

        void populateImagesList(bool added)
        {
            this->nbImages->setText("Images tacken: "+std::to_string(this->images.size()));
            const unsigned short IMGPREV_HEIGHT = 70;
            if (added)
                this->list->addComponent(
                    new ImagePreview(sf::Vector2i(5, 5), sf::Vector2i(this->list->getSize().x-10, IMGPREV_HEIGHT), *this->images[this->images.size()-1], "Image "+std::to_string(this->images.size()))
                );
            else
                this->list->removeComponent(this->list->getElements().size()-1);
            this->list->onScroll(IMGPREV_HEIGHT*4*this->images.size());
            this->list->onScroll(IMGPREV_HEIGHT*-8*this->images.size());
        }
        
        void setCalibrating(bool state)
        {
            this->generateTexture();
            this->calibrating = state;
            if (this->calibrating && !this->loadPopupDisplayed)
            {
                this->lf = new LoadingFrame(
                    sf::Vector2i(this->popup->getWinSize().x/2-200, this->popup->getWinSize().y/2-60),
                    sf::Vector2i(400, 120), CONSTANT::COLOR_BACK
                );
                this->popup->addComponent(this->lf);
                this->loadPopupDisplayed = true;
            } else if (this->loadPopupDisplayed)
            {
                this->popup->removeComponent(this->lf);
                this->loadPopupDisplayed = false;
            }
        }

        void changeLoadText(std::string txt)
        {
            this->shouldChangeText = true;
            this->newText = txt;
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        
    public:
        CalibrationPopup(Camera* camera)
        {
            this->setDimensions(0, 0, CONSTANT::WINDOW_WIDTH, CONSTANT::WINDOW_HEIGHT);
            this->temp.create(this->getSize().x, this->getSize().y);
            this->setReceiveEvents(true);
            this->setClearColor(CONSTANT::COLOR_TRANS);
            this->cam = camera;
            this->generateTexture();
            this->popup = new Popup("Camera calibration");
            this->addElement(this->popup);

            this->preview = new Image(sf::Vector2i(20, 20), sf::Vector2i(this->popup->getWinSize().x/2-40, this->popup->getWinSize().y-80), CONSTANT::COLOR_CLEAR);
            this->preview->setFlipped(true);
            this->cam->attachImage(this->preview);
            this->popup->addComponent(this->preview);

            this->nbImages = new Label("Images tacken: 0", sf::Vector2i(20, this->popup->getWinSize().y-60), sf::Vector2i(this->popup->getWinSize().x/2-40, 40), 18, Label::LEFT);
            this->popup->addComponent(this->nbImages);

            this->list = new List(sf::Vector2i(this->popup->getWinSize().x/2+10, 10), sf::Vector2i(this->popup->getWinSize().x/2-20, this->popup->getWinSize().y-130));
            this->popup->addComponent(this->list);

            this->addBtn = new Button("Add", sf::Vector2i(this->popup->getWinSize().x/2+10, this->popup->getWinSize().y-120), sf::Vector2i(120, 40), 20, CONSTANT::COLOR_CLEAR);
            this->remBtn = new Button("Remove", sf::Vector2i(this->popup->getWinSize().x/2+130, this->popup->getWinSize().y-120), sf::Vector2i(120, 40), 20, CONSTANT::COLOR_CLEAR);
            this->addBtn->setCallback(&CalibrationPopup::_add_image, this);
            this->remBtn->setCallback(&CalibrationPopup::_rem_image, this);
            this->popup->addComponent(this->addBtn);
            this->popup->addComponent(this->remBtn);

            this->saveBtn = new Button("Calibrate", sf::Vector2i(this->popup->getWinSize().x-170, this->popup->getWinSize().y-60), sf::Vector2i(160, 50), 20);
            this->saveBtn->setCallback(&CalibrationPopup::_launch_calibration, this);
            this->popup->addComponent(this->saveBtn);

            this->loadPopupDisplayed = false;
            this->setCalibrating(false);
            this->shouldJoin = false;
            this->shouldChangeText = false;
            this->shouldDetectChess = true;
            this->calibrator = std::thread(&CalibrationPopup::_detect_chessboard, this);
        }

        void generateTexture()
        {
            this->renderTexture.display();
            this->temp.clear(this->getClearColor());
            this->temp.draw(sf::Sprite(this->renderTexture.getTexture()));
            this->temp.display();
            this->renderTexture.clear(this->getClearColor());
            this->sprite.setTexture(this->temp.getTexture());
        }

        void onClick(int btn, bool clicked)
        {
            this->clicked = clicked;
        }

        void onFocus(bool focused)
        {
            this->focused = focused;
        }

        void onHover(bool hovered)
        {
            this->hovered = hovered;
        }
        
        void onKey(int key, char c, bool pressed)
        {
            
        }

        void onScroll(int delta)
        {
            
        }

        void update(float dt, sf::Vector2i mousePos)
        {
            if (this->shouldJoin)
            {
                this->shouldJoin = false;
                this->calibrator.join();
                if (!this->calibrating && !this->shouldJoin && !this->shouldDetectChess)
                {
                    this->shouldDetectChess = true;
                    this->calibrator = std::thread(&CalibrationPopup::_detect_chessboard, this);
                }
            }
        }

        void _add_image()
        {
            cv::Mat* im = new cv::Mat();
            this->cam->getRawImage().copyTo(*im);
            this->images.push_back(std::unique_ptr<cv::Mat>(im));
            this->populateImagesList(true);
        }

        void _rem_image()
        {
            if (this->images.size() < 1) return;
            this->images.erase(this->images.end()-1);
            this->populateImagesList(false);
        }

        void _detect_chessboard()
        {
            while (this->shouldDetectChess)
            {
                cv::findChessboardCorners(this->cam->getRawImage(), cv::Size(8, 5), markers, cv::CALIB_CB_ADAPTIVE_THRESH | cv::CALIB_CB_NORMALIZE_IMAGE);
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
        }

        void _launch_calibration()
        {
            if (this->images.size() < 20) return;
            if (this->shouldDetectChess)
            {
                this->shouldDetectChess = false;
                this->markers.clear();
                this->calibrator.join();
            }
            if (!this->calibrating)
                this->calibrator = std::thread(&CalibrationPopup::_execute_calibration, this);
        }

        void _save_calib(std::string path)
        {
            this->calibData.saveToFile(path);
            this->cam->loadCalibration(path);
        }

        void _execute_calibration()
        {
            this->setCalibrating(true);
            this->changeLoadText("Setting up calibration");
            cv::Size boardSize(8, 5);
            float squareSize = 2.8;

            std::vector<cv::Mat> rVectors, tVectors;
            cv::Mat distCoeffs(8, 1, CV_64F);
            cv::Mat camMatrix(3, 3, CV_64F);
            std::vector<std::vector<cv::Point2f>> imgPoints(this->images.size());
            std::vector<std::vector<cv::Point3f>> objPoints(1);
            
            this->changeLoadText("Finding calibration points");
            unsigned short detected = 0;
            unsigned short shift = 0;
            for (int s = 0; s < this->images.size(); s++)
            {
                cv::Mat gray;
                cv::cvtColor(*this->images[s], gray, cv::COLOR_BGR2GRAY);
                bool result = cv::findChessboardCorners(gray, boardSize, imgPoints[s-shift], cv::CALIB_CB_ADAPTIVE_THRESH | cv::CALIB_CB_NORMALIZE_IMAGE);
                if (!result)
                {
                    imgPoints.erase(imgPoints.begin()+s-shift);
                    shift++;
                    continue;
                }
                cv::cornerSubPix(gray, imgPoints[s-shift], cv::Size(10, 10), cv::Size(-1, -1), cv::TermCriteria(cv::TermCriteria::EPS+cv::TermCriteria::COUNT, 10, 0.1));
                detected++;
            }

            std::string message = "";
            if (detected > 15)
            {
                this->changeLoadText("Calibrating camera");
                for (int i = 0; i < boardSize.height; i++)
                    for (int j = 0; j < boardSize.width; j++)
                        objPoints[0].push_back(cv::Point3f(j * squareSize, i * squareSize, 0.f));    
                objPoints.resize(imgPoints.size(), objPoints[0]);
                cv::calibrateCamera(objPoints, imgPoints, boardSize, camMatrix, distCoeffs, rVectors, tVectors);

                this->changeLoadText("Saving calibration");
                this->calibData.cameraMatrix = camMatrix;
                this->calibData.distanceCoefficients = distCoeffs;
                message = "Done";
            } else
            {
                message = "Error: Can't find chessboard in more than 15 images";
            }

            this->lf->stopLoading();
            this->changeLoadText(message);
            std::this_thread::sleep_for(std::chrono::seconds( (detected>15)? 1: 3 ));
            if (detected>15)
            {
                Files::setCallback(&CalibrationPopup::_save_calib, this);
                Files::askForFile();
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
            this->setCalibrating(false);
            this->shouldJoin = true;
        }

        Popup* getPopup()
        {
            return this->popup;
        }

        sf::Sprite getSprite(float dt)
        {
            if (this->shouldDetectChess)
                for(cv::Point2f p: this->markers)
                    cv::circle(this->cam->getRawImage(), p, 3, cv::Scalar(0, 0, 255), 3);
            if (this->shouldChangeText)
            {
                this->shouldChangeText = false;
                this->lf->setText(this->newText);
            }
            this->generateTexture();
            return this->sprite;
        }

        ~CalibrationPopup()
        {
            this->cam->detachImage(this->preview);
            this->setCalibrating(false);
            if (this->shouldDetectChess || this->calibrating)
            {
                this->shouldDetectChess = false;
                this->shouldJoin = false;
                this->calibrator.join();
            }
        }
    };
}