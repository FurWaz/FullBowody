#pragma once
#include "../constants.hpp"
#include "./graphicElement.hpp"
#include "./popup.hpp"

namespace owo
{
    class CalibrationPopup : public virtual GraphicElement
    {
    private:
        Popup* popup;
    public:
        CalibrationPopup()
        {
            this->setDimensions(0, 0, CONSTANT::WINDOW_WIDTH, CONSTANT::WINDOW_HEIGHT);
            this->setReceiveEvents(true);
            this->setClearColor(sf::Color(0, 0, 0, 96));
            this->generateTexture();
            this->popup = new Popup("Camera calibration");
            this->addElement(this->popup);
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

        Popup* getPopup()
        {
            return this->popup;
        }

        sf::Sprite getSprite(float dt)
        {
            return this->sprite;
        }

        ~CalibrationPopup()
        {

        }

    };
}