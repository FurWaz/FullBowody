#pragma once
#include <math.h>
#include "../../constants.hpp"
#include "./graphicElement.hpp"

namespace owo
{
    class Loading : public virtual GraphicElement
    {
    private:
        float progress;
        float speed;
        sf::Color color;
        bool showing;

        void init()
        {
            this->sprite.setTexture(this->renderTexture.getTexture());
            this->showing = true;
        }

    public:
        Loading()
        {
            this->progress = 0;
            this->speed = 1;
            this->color = CONSTANT::COLOR_FORE;
            this->setDimensions(0, 0, 100, 100);
            this->init();
        }

        Loading(sf::Vector2i pos, sf::Vector2i size, float speed = 1, sf::Color color = CONSTANT::COLOR_FORE, sf::Color clearColor = CONSTANT::COLOR_BACK)
        {
            this->progress = 0;
            this->speed = speed;
            this->color = color;
            this->clearColor = clearColor;
            this->setDimensions(pos.x, pos.y, size.x, size.y);
            this->init();
        }

        void generateTexture()
        {
            this->renderTexture.clear(this->clearColor);
            if (this->showing)
            {
                float pos = ((int)(this->progress * this->speed * this->getSize().x * 0.1) % this->getSize().x*2) - this->getSize().x;
                sf::RectangleShape rect(sf::Vector2f(this->getSize().x, this->getSize().y));
                rect.setPosition(pos, 0);
                this->renderTexture.draw(rect);
            }
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
            
        }

        void show() {this->showing = true;}
        void hide() {this->showing = false;}

        sf::Sprite getSprite(float dt)
        {
            this->progress += dt;
            this->generateTexture();
            return this->sprite;
        }

        ~Loading()
        {

        }

    };
}