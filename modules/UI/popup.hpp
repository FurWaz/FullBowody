#pragma once
#include "../constants.hpp"
#include "./graphicElement.hpp"
#include "./button.hpp"
#include "./label.hpp"

namespace owo
{
    class Popup : public virtual GraphicElement
    {
    private:
        static const int POPUP_MARGIN = 40;
        Button* closeBtn;
        Label* title;

    public:
        Popup(std::string title="")
        {
            this->setDimensions(POPUP_MARGIN, POPUP_MARGIN, CONSTANT::WINDOW_WIDTH-POPUP_MARGIN*2, CONSTANT::WINDOW_HEIGHT-POPUP_MARGIN*2);
            this->setReceiveEvents(true);
            this->setClearColor(CONSTANT::COLOR_CLEAR);
            this->generateTexture();

            this->closeBtn = new Button("Close", sf::Vector2i(this->getSize().x-100, 0), sf::Vector2i(100, 50), 20);
            this->title = new Label(title, sf::Vector2i(this->getSize().x/2-100, 0), sf::Vector2i(200, 50), 18, Label::CENTER, CONSTANT::COLOR_FORE, CONSTANT::COLOR_CLEAR);
            this->addElement(this->closeBtn);
            this->addElement(this->title);
            this->propagateParentAbsPos();
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

        Button* getCloseButton()
        {
            return this->closeBtn;
        }

        sf::Sprite getSprite(float dt)
        {
            return this->sprite;
        }

        ~Popup()
        {

        }

    };
}