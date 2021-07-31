#pragma once
#include "../constants.hpp"
#include "graphicElement.hpp"

namespace owo
{
    class FPSCounter : public virtual GraphicElement
    {
    private:
        float avgDelta;

    public:
        FPSCounter()
        {
            this->avgDelta = 0;
            setDimensions(0, 0, 200, 50);
        }

        FPSCounter(sf::Vector2i pos, sf::Vector2i size)
        {
            this->avgDelta = 0;
            setDimensions(pos.x, pos.y, size.x, size.y);
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
            this->avgDelta += (dt - this->avgDelta) * 0.2;
            std::string txt = std::to_string((int)(1 / this->avgDelta)) + " FPS";

            this->renderTexture.create(this->dimensions.width, this->dimensions.height);
            this->renderTexture.clear(CONSTANT::COLOR_TRANS);

            sf::Text text(txt, CONSTANT::FONT, this->getSize().y * 0.8f);
            text.setFillColor(sf::Color::White);
            this->renderTexture.draw(text);
            this->renderTexture.display();

            this->sprite.setTexture(this->renderTexture.getTexture());
            this->sprite.setPosition(sf::Vector2f(this->dimensions.left, this->dimensions.top));
            return this->sprite;
        }

        ~FPSCounter()
        {
            
        }

    };
}