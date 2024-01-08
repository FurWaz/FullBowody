#pragma once
#include "../constants.hpp"
#include "./essentials/graphicElement.hpp"
#include "./essentials/label.hpp"
#include "./essentials/loading.hpp"

namespace owo
{
    class LoadingFrame : public virtual GraphicElement
    {
    private:
        Label* lbl;
        Loading* load;

        void init()
        {
            this->lbl = new Label("Loading ...", sf::Vector2i(5, 5), sf::Vector2i(this->getSize().x-10, this->getSize().y/2), 16, Label::CENTER, CONSTANT::COLOR_FORE, this->getClearColor());
            this->load = new Loading(sf::Vector2i(10, this->getSize().y/2+10), sf::Vector2i(this->getSize().x-20, 2), 8.f, CONSTANT::COLOR_PRIMARY, this->getClearColor());
            this->addElement(this->lbl);
            this->addElement(this->load);
            this->propagateParentAbsPos();
        }

    public:
        LoadingFrame(sf::Vector2i pos, sf::Vector2i size, sf::Color clearColor = CONSTANT::COLOR_CLEAR)
        {
            this->setClearColor(clearColor);
            this->setDimensions(pos.x, pos.y, size.x, size.y);
            this->generateTexture();
            this->init();
        }

        void generateTexture()
        {
            this->renderTexture.clear(this->clearColor);
            sf::RectangleShape rect(sf::Vector2f(this->getSize().x-4, this->getSize().y-4));
            rect.setPosition(2, 2);
            rect.setOutlineThickness(2);
            rect.setOutlineColor(CONSTANT::COLOR_CLEAR);
            rect.setFillColor(CONSTANT::COLOR_TRANS);
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

        void setText(std::string text)
        {
            this->lbl->setText(text);
        }

        void stopLoading()
        {
            this->load->hide();
        }

        void startLoading()
        {
            this->load->show();
        }

        std::string getText()
        {
            return this->lbl->getText();
        }

        sf::Sprite getSprite(float dt)
        {
            return this->sprite;
        }

        ~LoadingFrame()
        {

        }

    };
}