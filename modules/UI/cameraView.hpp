#pragma once
#include "../constants.hpp"
#include "../files.hpp"
#include "./essentials/graphicElement.hpp"
#include "./essentials/image.hpp"
#include "./essentials/input.hpp"
#include "./essentials/checkbox.hpp"
#include "./essentials/button.hpp"
#include "./essentials/title.hpp"

#include "../engine/camera.hpp"
#include "../engine/cameraCalibrator.hpp"

namespace owo
{
    class CameraView : public virtual GraphicElement
    {
    private:
        Camera* cam;
        Image* im;
        Input* input;
        Checkbox* checkbox;
        Label* checkbox_text;
        Button* calibrateBtn;
        Button* loadBtn;
        Button* saveBtn;
        Button* detectBtn;
        std::string cameraSource;

        std::thread loaderThread;
        bool shouldJoinThread;
        float dt;

        void init()
        {
            this->renderTexture.create(this->dimensions.width, this->dimensions.height);
            this->renderTexture.clear(CONSTANT::COLOR_BACK);
            this->sprite.setTexture(this->renderTexture.getTexture());
            this->sprite.setPosition(this->dimensions.left, this->dimensions.top);

            this->im = new Image(
                sf::Vector2i(4, 44),
                sf::Vector2i(this->dimensions.width/2-8, this->dimensions.height-48)
            );
            this->input = new Input(
                this->cam->getPath(),
                sf::Vector2i(0, 0), sf::Vector2i(this->dimensions.width, 40),
                14, Input::CENTER, CONSTANT::COLOR_WHITE_LIGHT, "Enter camera address"
            );
            this->detectBtn = new Button(
                "Detect position",
                sf::Vector2i(this->dimensions.width/2, this->dimensions.height-50),
                sf::Vector2i(this->dimensions.width/2, 50), 18,
                CONSTANT::COLOR_BACK, CONSTANT::COLOR_PRIMARY
            );
            this->addElement(new Title(
                sf::Vector2i(this->dimensions.width/2, this->dimensions.height-80),
                sf::Vector2i(this->dimensions.width/2, 30),
                "Position", 18
            ));
            this->calibrateBtn = new Button(
                "Calibrate",
                sf::Vector2i(this->dimensions.width*0.5, this->dimensions.height-130),
                sf::Vector2i(this->dimensions.width*0.2, 50), 18,
                CONSTANT::COLOR_BACK, CONSTANT::COLOR_PRIMARY
            );
            this->loadBtn = new Button(
                "Load",
                sf::Vector2i(this->dimensions.width*0.7, this->dimensions.height-130),
                sf::Vector2i(this->dimensions.width*0.15, 50), 16,
                CONSTANT::COLOR_BACK, CONSTANT::COLOR_PRIMARY
            );
            this->saveBtn = new Button(
                "Save",
                sf::Vector2i(this->dimensions.width*0.85, this->dimensions.height-130),
                sf::Vector2i(this->dimensions.width*0.15, 50), 16,
                CONSTANT::COLOR_BACK, CONSTANT::COLOR_PRIMARY
            );
            this->addElement(new Title(
                sf::Vector2i(this->dimensions.width/2, 40),
                sf::Vector2i(this->dimensions.width/2, 30),
                "Developers", 18
            ));
            this->checkbox = new Checkbox(
                sf::Vector2i(this->dimensions.width/2+5, 70),
                sf::Vector2i(20, 20),
                CONSTANT::COLOR_PRIMARY
            );
            this->checkbox_text = new Label(
                "Debug mode",
                sf::Vector2i(this->dimensions.width/2+40, 70),
                sf::Vector2i(this->dimensions.width/2-35, 20),
                16, Label::LEFT, CONSTANT::COLOR_FORE
            );
            
            this->cam->attachImage(this->im);
            this->input->setCallback(&CameraView::openCameraSource, this);
            this->checkbox->setCallback(&Camera::setDebugMode, this->cam);
            this->loadBtn->setCallback(&CameraView::loadCameraCalibration, this);
            this->saveBtn->setCallback(&CameraView::saveCameraCalibration, this);
            this->calibrateBtn->setCallback(&CameraView::calibrateCamera, this);
            this->detectBtn->setCallback(&CameraView::detectCameraPosition, this);
            this->shouldJoinThread = false;
            
            this->addElement(this->im);
            this->addElement(this->input);
            this->addElement(this->checkbox);
            this->addElement(this->checkbox_text);
            this->addElement(this->calibrateBtn);
            this->addElement(this->detectBtn);
            this->addElement(this->loadBtn);
            this->addElement(this->saveBtn);
            this->propagateParentAbsPos();
            this->dt = 0;
        }

    public:
        CameraView()
        {
            this->cam = new Camera();
            this->setDimensions(0, 0, 120, 200);
            this->init();
        }

        CameraView(Camera* cam)
        {
            this->cam = cam;
            this->setDimensions(0, 0, 120, 200);
            this->init();
        }

        CameraView(Camera* cam, sf::Vector2i position, sf::Vector2i size)
        {
            this->cam = cam;
            this->setDimensions(position.x, position.y, size.x, size.y);
            this->init();
        }

        void generateTexture()
        {
            this->cam->updateFrame(this->dt);
            this->renderTexture.display();
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
            if (this->shouldJoinThread)
            {
                this->shouldJoinThread = false;
                this->loaderThread.join();
            }
        }

        sf::Sprite getSprite(float dt)
        {
            this->dt = dt;
            this->generateTexture();
            return this->sprite;
        }

        Camera* getCamera()
        {
            return this->cam;
        }

        Image* getImage()
        {
            return this->im;
        }

        Input* getInput()
        {
            return this->input;
        }

        void setCamera(Camera* cam)
        {
            this->cam = cam;
        }

        void setImage(Image* im)
        {
            this->im = im;
        }

        void openCameraSource(std::string path)
        {
            this->cam->openSource(path);
        }

        void _update_source()
        {
            std::string txt = this->input->getText();
            if (txt != "")
            {
                this->input->setLoading(true);
                this->cam->openSource(txt);
                this->input->setLoading(false);
            }
            this->shouldJoinThread = true;
        }

        void loadCameraCalibration()
        {
            Files::setCallback(&CameraView::_load_cam_calib, this);
            Files::askForFile();
        }

        void saveCameraCalibration()
        {
            Files::setCallback(&CameraView::_save_cam_calib, this);
            Files::askForFile();
        }

        void _load_cam_calib(std::string path)
        {
            this->cam->loadCalibration(path);
        }

        void _save_cam_calib(std::string path)
        {
            this->cam->saveCalibration(path);
        }

        void calibrateCamera()
        {
            CameraCalibrator::calibrateCamera(this->cam);
        }

        void detectCameraPosition()
        {
            this->cam->calculatePosition();
        }

        ~CameraView()
        {

        }

    };
}