#pragma once
#include "../constants.hpp"
#include "CallbackContainer.hpp"
#include "graphicElement.hpp"
#include "label.hpp"

namespace owo {
    class Input : public virtual GraphicElement
    {
    private:
        std::string text;
        Label* label;
        CallbackContainer* cont;

    public:
        static const int LEFT = Label::LEFT;
        static const int CENTER = Label::CENTER;
        static const int RIGHT = Label::RIGHT;

        Input()
        {
            this->setDimensions(0, 0, 200, 50);
            this->text = "Input";
            this->label = new Label(this->text, 16);
            this->generateTexture();
        }

        Input(std::string text, int fontSize = 16, int placement = Input::CENTER, sf::Color textColor = CONST::COLOR_PRIMARY)
        {
            this->setDimensions(0, 0, 200, 50);
            this->text = text;
            this->label = new Label(this->text, fontSize, placement, textColor);
            this->generateTexture();
        }

        Input(std::string text,
              sf::Vector2i position, sf::Vector2i size = sf::Vector2i(-1, -1),
              int fontSize = 16, int placement = Input::CENTER, sf::Color textColor = CONST::COLOR_PRIMARY)
        {
            this->setDimensions(position.x, position.y, size.x, size.y);
            this->text = text;
            this->label = new Label(this->text, sf::Vector2i(0, 0), size, fontSize, placement, textColor);
            std::cout << this->text.c_str() << std::endl;
            this->generateTexture();
        }

        void generateTexture()
        {
            this->label->setText(this->text);
            this->label->generateTexture();
            this->renderTexture.create(this->dimensions.width, this->dimensions.height);
            this->renderTexture.clear(CONST::COLOR_BACKGROUND);
            this->renderTexture.draw(this->label->getSprite(0));
            if (this->focused)
            {
                sf::RectangleShape highlightRect(sf::Vector2f(this->dimensions.width-4, this->dimensions.height-4));
                highlightRect.setPosition(sf::Vector2f(2, 2));
                highlightRect.setFillColor(CONST::COLOR_TRANS);
                highlightRect.setOutlineColor(this->label->getTextColor());
                highlightRect.setOutlineThickness(2);
                this->renderTexture.draw(highlightRect);
            }
            this->renderTexture.display();
            this->sprite.setTexture(this->renderTexture.getTexture());
            this->sprite.setPosition(this->dimensions.left, this->dimensions.top);
        }

        void onClick(int btn, bool clicked)
        {
            this->clicked = clicked;
        }

        void onFocus(bool focused)
        {
            this->focused = focused;
            this->generateTexture();
            if (!this->focused && this->cont != nullptr)
                this->cont->func();
        }

        void onHover(bool hovered)
        {
            this->hovered = hovered;
        }
        
        void onKey(int key, char c, bool pressed)
        {
            if (!pressed) return;
            if (key == sf::Keyboard::BackSpace)
            {
                if (this->text.size() > 0)
                    this->text = this->text.substr(0, this->text.size()-1);
            }
            else
                if (c != CONST::NO_CHAR)
                    this->text += c;
            this->generateTexture();
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

        std::string getText()
        {
            return this->text;
        }

        template<class T> void setCallback(void (T::*callback)(), T* instance)
        {
            this->cont = new TypedCallbackContainer<T>(callback, instance);
        }

        ~Input()
        {

        }

    };
}