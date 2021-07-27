#pragma once
#include "../constants.hpp"
#include "graphicElement.hpp"
#include "CallbackContainer.hpp"

namespace owo
{
    class Checkbox : public virtual GraphicElement
    {
    private:
        bool checked;
        sf::Color color;
        CallbackContainer* cont;

        void generateTexture()
        {
            this->renderTexture.create(this->dimensions.width, this->dimensions.height);
            this->renderTexture.clear(CONSTANT::COLOR_BACK);

            int oulineThickness = 2;
            sf::RectangleShape rect(sf::Vector2f(this->dimensions.width-oulineThickness*2, this->dimensions.height-oulineThickness*2));
            rect.setPosition(oulineThickness, oulineThickness);
            rect.setOutlineThickness(oulineThickness);
            rect.setOutlineColor(CONSTANT::COLOR_FORE);
            if (this->checked)
                rect.setFillColor(this->color);
            else
                rect.setFillColor(CONSTANT::COLOR_TRANS);
            this->renderTexture.draw(rect);
            this->sprite.setTexture(this->renderTexture.getTexture());
            this->sprite.setPosition(this->dimensions.left, this->dimensions.top);
        }

    public:
        Checkbox()
        {
            this->checked = false;
            this->setDimensions(0, 0, 100, 100);
            this->color = CONSTANT::COLOR_PRIMARY;
            this->generateTexture();
        }

        Checkbox(sf::Vector2i pos, sf::Vector2i size, sf::Color color = CONSTANT::COLOR_PRIMARY)
        {
            this->checked = false;
            this->setDimensions(pos.x, pos.y, size.x, size.y);
            this->color = color;
            this->generateTexture();
        }

        void onClick(int btn, bool clicked)
        {
            this->clicked = clicked;
            if (clicked)
            {
                this->checked = !this->checked;
                this->generateTexture();
                if (this->cont != nullptr)
                    this->cont->func();
            }
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

        bool isChecked()
        {
            return this->checked;
        }

        template<class T> void setCallback(void (T::*callback)(), T* instance)
        {
            this->cont = new TypedCallbackContainer<T>(callback, instance);
        }

        ~Checkbox()
        {

        }

    };
}