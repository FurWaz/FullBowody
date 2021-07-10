#pragma once
#include "../constants.hpp"
#include "graphicElement.hpp"

namespace owo {
    class Label : public virtual GraphicElement
    {
    private:
        std::string text;
        sf::Color textColor;
        int fontSize;
        int placement;

    public:
        void generateTexture()
        {
            sf::Text txt(this->text, CONST::FONT, this->fontSize);
            txt.setFillColor(this->textColor);
            sf::Vector2u txtSize(txt.getLocalBounds().width, this->fontSize);

            if (this->dimensions.width == -1 || this->dimensions.height == -1)
                this->dimensions = sf::IntRect(this->dimensions.left, this->dimensions.top, txtSize.x, txtSize.y);
            
            this->renderTexture.create(this->dimensions.width, this->dimensions.height);
            this->renderTexture.clear(CONST::COLOR_BACKGROUND);
            switch (this->placement)
            {
            case Label::CENTER:
                txt.setPosition(
                    (this->dimensions.width - txtSize.x) / 2,
                    (this->dimensions.height - txtSize.y) / 2
                );
                break;
            case Label::RIGHT:
                txt.setPosition(
                    this->dimensions.width - txtSize.x,
                    (this->dimensions.height - txtSize.y) / 2
                );
                break;
            case Label::LEFT:
                txt.setPosition(
                    0,
                    (this->dimensions.height - txtSize.y) / 2
                );
                break;
            default:
                break;
            }
            this->renderTexture.draw(txt);
            this->renderTexture.display();
            this->sprite.setTexture(this->renderTexture.getTexture());
            this->sprite.setPosition(this->dimensions.left, this->dimensions.top);
        }

        static const int LEFT = 1;
        static const int CENTER = 2;
        static const int RIGHT = 3;

        Label()
        {
            this->text = "Label";
            this->fontSize = 16;
            this->setDimensions(0, 0, -1, -1);
            this->generateTexture();
        }

        Label(std::string text, int fontSize = 16, int placement = Label::CENTER, sf::Color textColor = CONST::COLOR_FOREGROUND)
        {
            this->text = text;
            this->fontSize = fontSize;
            this->placement = placement;
            this->textColor = textColor;
            this->setDimensions(0, 0, -1, -1);
            this->generateTexture();
        }

        Label(std::string text,
              sf::Vector2i position, sf::Vector2i size = sf::Vector2i(-1, -1),
              int fontSize = 16, int placement = Label::CENTER, sf::Color textColor = CONST::COLOR_FOREGROUND)
        {
            this->text = text;
            this->fontSize = fontSize;
            this->placement = placement;
            this->textColor = textColor;
            this->setDimensions(position.x, position.y, size.x, size.y);
            this->generateTexture();
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

        void setText(std::string text)
        {
            this->text = text;
            this->generateTexture();
        }

        sf::Sprite getSprite(float dt)
        {
            return this->sprite;
        }

        sf::Color getTextColor()
        {
            return this->textColor;
        }

        ~Label()
        {

        }

    };
}