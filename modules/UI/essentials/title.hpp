#pragma once
#include "../../constants.hpp"
#include "./graphicElement.hpp"
#include "./label.hpp"

namespace owo
{
    class Title : public virtual GraphicElement
    {
    private:
        Label* label;

    public:
        Title(sf::Vector2i pos, sf::Vector2i size, std::string str = "Title", int fontSize = 16, sf::Color clearColor = CONSTANT::COLOR_BACK)
        {
            this->setDimensions(pos.x, pos.y, size.x, size.y);
            this->setClearColor(clearColor);
            this->label = new Label(str, sf::Vector2i(0, 0), sf::Vector2i(-1, size.y), fontSize, Label::CENTER, CONSTANT::COLOR_FORE, this->clearColor);
            this->label->setPosition(sf::Vector2i((size.x-this->label->getSize().x)/2, 0));
            this->label->generateTexture();
            this->addElement(this->label);
            this->generateTexture();
            this->propagateParentAbsPos();
        }

        void generateTexture()
        {
            this->renderTexture.clear(this->clearColor);
            sf::RectangleShape rect;
            rect.setPosition(2, this->dimensions.height/2);
            rect.setSize(sf::Vector2f(this->dimensions.width-4, 2));
            rect.setFillColor(CONSTANT::COLOR_GREY_DARK);
            this->renderTexture.draw(rect);
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

        ~Title()
        {

        }

    };
}