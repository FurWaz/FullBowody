#pragma once
#include "../../constants.hpp"
#include "./graphicElement.hpp"

namespace owo
{
    class Template : public virtual GraphicElement
    {
    private:

    public:
        Template()
        {
            
        }

        void generateTexture()
        {

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

        ~Template()
        {

        }

    };
}