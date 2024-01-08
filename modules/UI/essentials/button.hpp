#pragma once
#include "../../constants.hpp"
#include "./graphicElement.hpp"
#include "./callbackContainer.hpp"
#include "./label.hpp"
#include "./loading.hpp"

namespace owo
{
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
        int fontSize;

        Label* label;
        
        CallbackContainer* cont;
        bool loading;
        bool shouldLoad;
        Loading* loadLogo;

        void init()
        {
            this->generateHoverColors();
            this->label = new Label(this->text, sf::Vector2i(), this->getSize(), this->fontSize, Label::CENTER, this->fgColor, this->bgColor);
            this->loadLogo = new Loading(sf::Vector2i(0, this->getSize().y-4), sf::Vector2i(this->getSize().x, 4));
            this->shouldLoad = false;
            this->loading = false;
            this->setReceiveEvents(true);
        }

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

    public:
        Button()
        {
            this->bgColor_normal = CONSTANT::COLOR_BACK;
            this->fgColor_normal = CONSTANT::COLOR_FORE;
            this->text = "Button";
            this->fontSize = 18;
            this->setDimensions(0, 0, 140, 40);
            init();
            generateTexture();
        }

        Button(std::string text, sf::Vector2i pos, sf::Vector2i size)
        {
            this->bgColor_normal = CONSTANT::COLOR_BACK;
            this->fgColor_normal = CONSTANT::COLOR_FORE;
            this->text = text;
            this->fontSize = 18;
            this->setDimensions(pos.x, pos.y, size.x, size.y);
            init();
            generateTexture();
        }

        Button(std::string text, sf::Vector2i pos, sf::Vector2i size, int fontSize, sf::Color BGColor = CONSTANT::COLOR_BACK, sf::Color FGColor = CONSTANT::COLOR_PRIMARY)
        {
            this->bgColor_normal = BGColor;
            this->fgColor_normal = FGColor;
            this->text = text;
            this->fontSize = fontSize;
            this->setDimensions(pos.x, pos.y, size.x, size.y);
            init();
            generateTexture();
        }

        void generateTexture()
        {
            this->renderTexture.create(this->dimensions.width, this->dimensions.height);
            this->renderTexture.clear(this->bgColor);

            this->label->setTextColor(this->fgColor);
            this->label->setClearColor(this->bgColor);
            this->label->setText(this->text);
            this->renderTexture.draw(this->label->getSprite(0));

            this->loading = this->shouldLoad;
            if (this->loading)
                this->renderTexture.draw(this->loadLogo->getSprite(0.16));

            this->renderTexture.display();
            this->sprite.setTexture(this->renderTexture.getTexture());
            this->sprite.setPosition(sf::Vector2f(this->dimensions.left, this->dimensions.top));
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
            if (pressed && key == sf::Keyboard::Space)
                this->onClick(0, true);
        }

        void onScroll(int delta)
        {
            
        }

        void update(float dt, sf::Vector2i mousePos)
        {
            
        }

        void setLabel(Label* label)
        {
            this->label = label;
        }

        void setText(std::string str)
        {
            this->text = str;
            this->generateTexture();
        }

        void setTextColor(sf::Color color)
        {
            this->fgColor_normal = color;
            this->generateHoverColors();
            this->generateTexture();
        }

        void setBackColor(sf::Color color)
        {
            this->bgColor_normal = color;
            this->generateHoverColors();
            this->generateTexture();
        }

        void setLoading(bool state)
        {
            this->shouldLoad = state;
        }

        bool isLoading()
        {
            return this->shouldLoad;
        }

        Label* getLabel()
        {
            if (this->shouldLoad || this->loading != this->shouldLoad)
                this->generateTexture();
            return this->label;
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