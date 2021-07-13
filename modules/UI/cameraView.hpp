#pragma once
#include "../constants.hpp"
#include "graphicElement.hpp"
#include "image.hpp"
#include "input.hpp"
#include "../engine/camera.hpp"

namespace owo
{
    class CameraView : public virtual GraphicElement
    {
    private:
        Camera* cam;
        Image* im;
        Input* input;
        std::string cameraSource;

        void setupCamView()
        {
            this->im->setDimensions(this->dimensions.left+10, this->dimensions.top+50, this->dimensions.width - 20, this->dimensions.height - 60);
            this->input = new Input(
                this->cam->getPath(),
                sf::Vector2i(this->dimensions.left, this->dimensions.top), sf::Vector2i(this->dimensions.width, 40),
                14, Input::CENTER, CONST::COLOR_PRIMARY, "Enter camera address"
            );
            this->input->setCallback(&CameraView::_update_source, this);
            this->renderTexture.create(this->dimensions.width, this->dimensions.height);
            this->renderTexture.clear(CONST::COLOR_BACKGROUND);
            this->sprite.setTexture(this->renderTexture.getTexture());
            this->sprite.setPosition(this->dimensions.left, this->dimensions.top);
        }

    public:
        CameraView()
        {
            this->im = new Image();
            this->input = new Input();
            this->cam = new Camera(this->im);
            this->setDimensions(0, 0, 120, 200);
            this->setupCamView();
        }

        CameraView(Camera* cam)
        {
            this->im = new Image();
            this->cam = cam;
            this->cam->attachImage(this->im);
            this->setDimensions(0, 0, 120, 200);
            this->setupCamView();
        }

        CameraView(Camera* cam, sf::Vector2i position, sf::Vector2i size)
        {
            this->im = new Image();
            this->cam = cam;
            this->cam->attachImage(this->im);
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
            this->cam->updateFrame();
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

        void _update_source()
        {
            this->cam->openSource(this->input->getText());
        }

        ~CameraView()
        {

        }

    };
}