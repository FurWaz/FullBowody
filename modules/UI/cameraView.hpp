#pragma once
#include "../constants.hpp"
#include "graphicElement.hpp"
#include "image.hpp"
#include "input.hpp"
#include "checkbox.hpp"
#include "button.hpp"
#include "../engine/camera.hpp"

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

        void setupCamView()
        {
            this->renderTexture.create(this->dimensions.width, this->dimensions.height);
            this->renderTexture.clear(CONSTANT::COLOR_BACK);
            this->sprite.setTexture(this->renderTexture.getTexture());
            this->sprite.setPosition(this->dimensions.left, this->dimensions.top);

            this->im = new Image(
                sf::Vector2i(this->dimensions.left+4, this->dimensions.top+50),
                sf::Vector2i(this->dimensions.width-8, this->dimensions.height-200)
            );
            this->input = new Input(
                this->cam->getPath(),
                sf::Vector2i(this->dimensions.left, this->dimensions.top), sf::Vector2i(this->dimensions.width, 40),
                14, Input::CENTER, CONSTANT::COLOR_WHITE_LIGHT, "Enter camera address"
            );
            this->input->setCallback(&CameraView::_update_source, this);
            this->detectBtn = new Button(
                "Detect position",
                sf::Vector2i(this->dimensions.left, this->dimensions.top+this->dimensions.height-50),
                sf::Vector2i(this->dimensions.width, 50), 18,
                CONSTANT::COLOR_BACK, CONSTANT::COLOR_PRIMARY
            );
            this->calibrateBtn = new Button(
                "Calibrate",
                sf::Vector2i(this->dimensions.left, this->dimensions.top+this->dimensions.height-100),
                sf::Vector2i(this->dimensions.width*0.4, 50), 18,
                CONSTANT::COLOR_BACK, CONSTANT::COLOR_PRIMARY
            );
            this->loadBtn = new Button(
                "Load",
                sf::Vector2i(this->dimensions.left+this->dimensions.width*0.4, this->dimensions.top+this->dimensions.height-100),
                sf::Vector2i(this->dimensions.width*0.3, 50), 16,
                CONSTANT::COLOR_BACK, CONSTANT::COLOR_PRIMARY
            );
            this->saveBtn = new Button(
                "Save",
                sf::Vector2i(this->dimensions.left+this->dimensions.width*0.7, this->dimensions.top+this->dimensions.height-100),
                sf::Vector2i(this->dimensions.width*0.3, 50), 16,
                CONSTANT::COLOR_BACK, CONSTANT::COLOR_PRIMARY
            );
            this->checkbox = new Checkbox(
                sf::Vector2i(this->dimensions.left+10, this->dimensions.top+this->dimensions.height-135),
                sf::Vector2i(20, 20),
                CONSTANT::COLOR_PRIMARY
            );
            this->checkbox_text = new Label(
                "Debug mode",
                sf::Vector2i(this->dimensions.left+40, this->dimensions.top+this->dimensions.height-140),
                sf::Vector2i(this->dimensions.width-50, 30),
                16, Label::LEFT, CONSTANT::COLOR_FORE
            );
            
            this->cam->attachImage(this->im);
            this->checkbox->setCallback(&CameraView::toogleDebugMode, this);
            this->loadBtn->setCallback(&CameraView::loadCameraCalibration, this);
            this->saveBtn->setCallback(&CameraView::saveCameraCalibration, this);
            this->calibrateBtn->setCallback(&CameraView::calibrateCamera, this);
            this->detectBtn->setCallback(&CameraView::detectCameraPosition, this);
        }

    public:
        CameraView()
        {
            this->cam = new Camera();
            this->setDimensions(0, 0, 120, 200);
            this->setupCamView();
        }

        CameraView(Camera* cam)
        {
            this->cam = cam;
            this->setDimensions(0, 0, 120, 200);
            this->setupCamView();
        }

        CameraView(Camera* cam, sf::Vector2i position, sf::Vector2i size)
        {
            this->cam = cam;
            this->setDimensions(position.x, position.y, size.x, size.y);
            this->setupCamView();
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
            
        }

        sf::Sprite getSprite(float dt)
        {
            this->cam->updateFrame(dt);
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

        std::vector<GraphicElement*> getElements()
        {
            std::vector<GraphicElement*> result;
            result.push_back(this->im);
            result.push_back(this->input);
            result.push_back(this->checkbox);
            result.push_back(this->checkbox_text);
            result.push_back(this->calibrateBtn);
            result.push_back(this->detectBtn);
            result.push_back(this->loadBtn);
            result.push_back(this->saveBtn);
            return result;
        }

        void setCamera(Camera* cam)
        {
            this->cam = cam;
        }

        void setImage(Image* im)
        {
            this->im = im;
        }

        void _update_source()
        {
            std::string txt = this->input->getText();
            if (txt != "")
                this->cam->openSource(txt);
        }

        void loadCameraCalibration()
        {
            std::cout << "this->cam->loadCalibration(path);" << std::endl;
        }

        void saveCameraCalibration()
        {
            std::cout << "this->cam->saveCalibration(path);" << std::endl;
        }

        void calibrateCamera()
        {
            std::cout << "this->cam->calibrate();" << std::endl;
        }

        void detectCameraPosition()
        {
            this->cam->calculatePosition();
        }

        void toogleDebugMode()
        {
            this->cam->setDebugMode( this->checkbox->isChecked() );
        }

        ~CameraView()
        {

        }

    };
}