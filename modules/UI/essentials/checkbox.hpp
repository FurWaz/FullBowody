#pragma once
#include "../../constants.hpp"
#include "./graphicElement.hpp"
#include "./CallbackContainer.hpp"

namespace owo
{
    class Checkbox : public virtual GraphicElement
    {
    private:
        bool checked;
        sf::Color color;
        BoolCallbackContainer* cont;

        void init()
        {
            this->setReceiveEvents(true);
        }

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
            this->setClearColor(CONSTANT::COLOR_BACK);
            this->color = CONSTANT::COLOR_PRIMARY;
            this->init();
            this->generateTexture();
        }

        Checkbox(sf::Vector2i pos, sf::Vector2i size, sf::Color color = CONSTANT::COLOR_PRIMARY, sf::Color clearColor = CONSTANT::COLOR_BACK)
        {
            this->checked = false;
            this->setDimensions(pos.x, pos.y, size.x, size.y);
            this->setClearColor(clearColor);
            this->color = color;
            this->init();
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
                    this->cont->func(this->checked);
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

        void setChecked(bool state)
        {
            this->checked = state;
            this->generateTexture();
        }

        bool isChecked()
        {
            return this->checked;
        }

        template<class T> void setCallback(void (T::*callback)(bool), T* instance)
        {
            this->cont = new TypedBoolCallbackContainer<T>(callback, instance);
        }

        ~Checkbox()
        {

        }

    };
}