#pragma once
#include "../../constants.hpp"
#include "./graphicElement.hpp"

namespace owo
{
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
            sf::Text txt(this->text, CONSTANT::FONT, this->fontSize);
            txt.setFillColor(this->textColor);
            
            this->renderTexture.clear(this->clearColor);
            txt.setPosition(this->calculateTextPos(this->text));
            this->renderTexture.draw(txt);
            this->renderTexture.display();
            this->sprite.setTexture(this->renderTexture.getTexture());
        }

        static const int LEFT = 1;
        static const int CENTER = 2;
        static const int RIGHT = 3;

        Label()
        {
            this->text = "Label";
            this->fontSize = 16;
            this->setDimensions(0, 0, 20, 20);
            this->generateTexture();
        }

        Label(std::string text, int fontSize = 16, int placement = Label::CENTER, sf::Color textColor = CONSTANT::COLOR_FORE, sf::Color clearColor = CONSTANT::COLOR_BACK)
        {
            this->text = text;
            this->fontSize = fontSize;
            this->placement = placement;
            this->textColor = textColor;
            this->clearColor = clearColor;
            this->setDimensions(0, 0, 20, 20);
            this->generateTexture();
        }

        Label(std::string text, sf::Vector2i position, sf::Vector2i size = sf::Vector2i(0, 0),
              int fontSize = 16, int placement = Label::CENTER, sf::Color textColor = CONSTANT::COLOR_FORE, sf::Color clearColor = CONSTANT::COLOR_BACK)
        {
            this->text = text;
            this->fontSize = fontSize;
            this->placement = placement;
            this->textColor = textColor;
            this->clearColor = clearColor;
            sf::Vector2u txtSize = this->calculateTextSize(this->text);
            this->setDimensions(position.x, position.y, (size.x == -1)? txtSize.x+10: size.x, (size.y == -1)? txtSize.y+4: size.y);
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
        
        sf::Vector2f calculateTextPos(std::string str)
        {
            sf::Text txt(str, CONSTANT::FONT, this->fontSize);
            sf::Vector2u txtSize(txt.getLocalBounds().width, this->fontSize);
            sf::Vector2f result;
            switch (this->placement)
            {
            case Label::CENTER:
                result = sf::Vector2f(
                    (this->dimensions.width - txtSize.x) / 2,
                    (this->dimensions.height - txtSize.y) / 2 - 2
                );
                break;
            case Label::RIGHT:
                result = sf::Vector2f(
                    this->dimensions.width - txtSize.x - 1,
                    (this->dimensions.height - txtSize.y) / 2 - 2
                );
                break;
            case Label::LEFT:
                result = sf::Vector2f(
                    1,
                    (this->dimensions.height - txtSize.y) / 2 - 2
                );
                break;
            default:
                break;
            }
            return result;
        }

        sf::Vector2u calculateTextSize(std::string str)
        {
            sf::Text txt(str, CONSTANT::FONT, this->fontSize);
            return sf::Vector2u(txt.getLocalBounds().width, this->fontSize);
        }

        void setFontSize(int size)
        {
            this->fontSize = size;
            this->generateTexture();
        }

        void setPlacement(int placement)
        {
            this->placement = placement;
            this->generateTexture();
        }

        void setTextColor(sf::Color color)
        {
            this->textColor = color;
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

        int getFontSize()
        {
            return this->fontSize;
        }

        int getPlacement()
        {
            return this->placement;
        }

        std::string getText()
        {
            return this->text;
        }

        ~Label()
        {

        }

    };
}