#pragma once
#include "../constants.hpp"
#include "CallbackContainer.hpp"
#include "graphicElement.hpp"

namespace owo {
    class Button : public virtual GraphicElement
    {
    private:
        std::string text;
        sf::Color fgColor;
        sf::Color bgColor;
        sf::Color bgColor_normal;
        sf::Color fgColor_normal;
        sf::Color fgColor_hover;
        sf::Color bgColor_hover;
        int textSize;
        
        CallbackContainer* cont;

        void generateHoverColors()
        {
            this->fgColor_hover = sf::Color(
                std::min((int)(this->fgColor_normal.r*1.2), 255),
                std::min((int)(this->fgColor_normal.g*1.2), 255),
                std::min((int)(this->fgColor_normal.b*1.2), 255)
            );
            this->bgColor_hover = sf::Color(
                std::min((int)(this->bgColor_normal.r*1.2), 255),
                std::min((int)(this->bgColor_normal.g*1.2), 255),
                std::min((int)(this->bgColor_normal.b*1.2), 255)
            );
            this->bgColor = this->bgColor_normal;
            this->fgColor = this->fgColor_normal;
        }

        void generateTexture()
        {
            this->renderTexture.create(this->dimensions.width, this->dimensions.height);
            this->renderTexture.clear(this->bgColor);

            sf::Text text(this->text, CONST::FONT, this->textSize);
            text.setFillColor(this->fgColor);
            text.setPosition(
                (this->dimensions.width-text.getLocalBounds().width)/2,
                (this->dimensions.height-this->textSize)/2
            );
            this->renderTexture.draw(text);

            this->renderTexture.display();
            this->sprite.setTexture(this->renderTexture.getTexture());
            this->sprite.setPosition(sf::Vector2f(this->dimensions.left, this->dimensions.top));
        }

    public:
        Button()
        {
            this->bgColor_normal = CONST::COLOR_BACKGROUND;
            this->fgColor_normal = CONST::COLOR_FOREGROUND;
            this->text = "Button";
            this->textSize = 18;
            this->setDimensions(0, 0, 140, 40);
            generateHoverColors();
            generateTexture();
        }

        Button(std::string text, sf::Vector2i pos, sf::Vector2i size)
        {
            this->bgColor_normal = CONST::COLOR_BACKGROUND;
            this->fgColor_normal = CONST::COLOR_FOREGROUND;
            this->text = text;
            this->textSize = 18;
            this->setDimensions(pos.x, pos.y, size.x, size.y);
            generateHoverColors();
            generateTexture();
        }

        Button(std::string text, sf::Vector2i pos, sf::Vector2i size, sf::Color BGColor, sf::Color FGColor)
        {
            this->bgColor_normal = BGColor;
            this->fgColor_normal = FGColor;
            this->text = text;
            this->textSize = 18;
            this->setDimensions(pos.x, pos.y, size.x, size.y);
            generateHoverColors();
            generateTexture();
        }

        template<class T> void setCallback(void (T::*callback)(), T* c)
        {
            this->cont = new TypedCallbackContainer<T>(callback, c);
        }

        void setCallback(void (*callback)())
        {
            this->cont = new VoidCallbackContainer(callback);
        }

        void onClick(int btn, bool clicked)
        {
            if (btn != sf::Mouse::Left) return;
            this->clicked = clicked;
            if (clicked && this->cont != nullptr)
                this->cont->func();
        }

        void onFocus(bool focused)
        {
            this->focused = focused;
        }

        void onHover(bool hovered)
        {
            this->hovered = hovered;

            if (hovered)
            {
                this->bgColor = this->bgColor_hover;
                this->fgColor = this->fgColor_hover;
            } else
            {
                this->bgColor = this->bgColor_normal;
                this->fgColor = this->fgColor_normal;
            }
            generateTexture();
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

        ~Button()
        {

        }
    };
}