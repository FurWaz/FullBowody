#pragma once
#include <opencv2/core.hpp>
#include "../constants.hpp"
#include "./essentials/graphicElement.hpp"
#include "./essentials/label.hpp"
#include "./essentials/image.hpp"

namespace owo
{
    class ImagePreview : public virtual GraphicElement
    {
    private:
        Image* img;
        Label* lbl;

        void init(cv::Mat image, std::string title)
        {
            cv::Mat rgba;
            cv::cvtColor(image, rgba, cv::COLOR_BGR2RGBA);
            this->img = new Image(sf::Vector2i(5, 5), sf::Vector2i(this->getSize().y-10, this->getSize().y-10), CONSTANT::COLOR_CLEAR);
            this->img->fromArray(rgba.ptr(), sf::Vector2u(image.cols, image.rows));
            this->lbl = new Label(title, sf::Vector2i(this->getSize().y, 5), sf::Vector2i(this->getSize().x-this->getSize().y, this->getSize().y-10), 16, Label::LEFT);
            this->addElement(this->img);
            this->addElement(this->lbl);
            this->propagateParentAbsPos();
        }

    public:
        ImagePreview()
        {
            this->setDimensions(0, 0, 100, 50);
            this->setClearColor(CONSTANT::COLOR_BACK);
            this->init(cv::Mat(), "");
        }
        
        ImagePreview(sf::Vector2i pos, sf::Vector2i size, cv::Mat image, std::string title, sf::Color clearColor = CONSTANT::COLOR_BACK)
        {
            this->setDimensions(pos.x, pos.y, size.x, size.y);
            this->setClearColor(clearColor);
            this->init(image, title);
        }

        void generateTexture()
        {
            this->renderTexture.clear(this->clearColor);
            this->renderTexture.display();
            this->sprite.setTexture(this->renderTexture.getTexture());
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
            return this->sprite;
        }

        ~ImagePreview()
        {

        }

    };
}