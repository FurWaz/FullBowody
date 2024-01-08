#pragma once
#include "../constants.hpp"
#include "./essentials/graphicElement.hpp"
#include "./essentials/label.hpp"

#include "../engine/softwareConnection.hpp"

namespace owo
{
    class SoftwareInfoPanel : public virtual GraphicElement
    {
    private:
        std::string name;
        std::string ip;
        unsigned short port;

        void init()
        {
            this->addElement(new Label(
                this->name,
                sf::Vector2i(5, 5), sf::Vector2i(this->getSize().x-10, 30),
                20, Label::LEFT, CONSTANT::COLOR_PRIMARY, this->getClearColor()
            ));
            this->addElement(new Label(
                "IP: "+this->ip,
                sf::Vector2i(15, 40), sf::Vector2i(this->getSize().x-20, 25),
                18, Label::LEFT, CONSTANT::COLOR_FORE, this->getClearColor()
            ));
            this->addElement(new Label(
                "PORT: "+std::to_string(this->port),
                sf::Vector2i(15, 70), sf::Vector2i(this->getSize().x-20, 25),
                18, Label::LEFT, CONSTANT::COLOR_FORE, this->getClearColor()
            ));
        }

    public:
        SoftwareInfoPanel(sf::Vector2i pos, sf::Vector2i size, SoftwareInfo inf = SoftwareInfo(), sf::Color clearColor = CONSTANT::COLOR_BACK)
        {
            this->port = inf.port;
            this->name = inf.name;
            this->ip = inf.ip;
            this->setDimensions(pos.x, pos.y, size.x, size.y);
            this->setClearColor(clearColor);
            this->init();
            this->generateTexture();
        }

        void generateTexture()
        {
            this->renderTexture.clear(this->getClearColor());
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

        ~SoftwareInfoPanel()
        {

        }
    };
}