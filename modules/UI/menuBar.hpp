#pragma once
#include "../constants.hpp"
#include "./button.hpp"
#include "graphicElement.hpp"

namespace owo
{
    class MenuBar : public virtual GraphicElement
    {
    private:
        Button* fileBtn;
        Button* optsBtn;
        Button* moreBtn;

    public:
        MenuBar()
        {
            this->setDimensions(0, 0, CONSTANT::WINDOW_WIDTH, 20);
            this->fileBtn = new Button("File", sf::Vector2i(0, 0), sf::Vector2i(100, this->getSize().y));
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

        ~MenuBar()
        {

        }

    };
}