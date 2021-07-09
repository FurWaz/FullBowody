#pragma once
#include "constants.hpp"
#include "graphicElement.hpp"
#include "image.hpp"
#include "../engine/camera.hpp"

namespace owo {
    class CameraView : public virtual GraphicElement
    {
    private:
        Camera* cam;
        Image* im;

        float delta = 0;

        void setupCamView()
        {
            this->im->setDimensions(10, 10, this->dimensions.width - 20, this->dimensions.height - 60);
        }

    public:
        CameraView()
        {
            this->im = new Image();
            this->cam = new Camera(this->im);
            this->setDimensions(0, 0, 120, 200);
            this->setupCamView();
        }

        CameraView(Image* im)
        {
            this->im = im;
            this->cam = new Camera(this->im);
            this->setDimensions(0, 0, 120, 200);
            this->setupCamView();
        }

        CameraView(Camera* cam)
        {
            this->im = new Image;
            this->cam = cam;
            this->cam->attachImage(this->im);
            this->setDimensions(0, 0, 120, 200);
            this->setupCamView();
        }

        CameraView(Image* im, Camera* cam)
        {
            this->im = im;
            this->cam = cam;
            this->cam->attachImage(this->im);
            this->setDimensions(0, 0, 120, 200);
            this->setupCamView();
        }

        CameraView(Camera* cam, sf::Vector2i position, sf::Vector2u size)
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
            this->delta += dt;
            if (this->delta >= 1/CONST::CAMERA_FPS)
            {
                this->delta = 0;
                this->cam->readFrame();
            }
        }

        sf::Sprite getSprite(float dt)
        {
            this->renderTexture.create(this->dimensions.width, this->dimensions.height);
            this->renderTexture.clear(CONST::COLOR_BLACK);
            this->renderTexture.draw(this->im->getSprite(dt));
            this->sprite.setTexture(this->renderTexture.getTexture());
            this->sprite.setPosition(this->dimensions.left, this->dimensions.top);
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

        void setCamera(Camera* cam)
        {
            this->cam = cam;
        }

        void setImage(Image* im)
        {
            this->im = im;
        }

        ~CameraView()
        {

        }

    };
}