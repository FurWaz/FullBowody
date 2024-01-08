#pragma once
#include "../constants.hpp"
#include "./essentials/graphicElement.hpp"
#include "./essentials/button.hpp"
#include "./essentials/label.hpp"

namespace owo
{
    class Popup : public virtual GraphicElement
    {
    private:
        static const int POPUP_MARGIN = 50;
        static const int HEADER_SIZE = 40;
        static const int SHADOW_SIZE = 5;
        Button* closeBtn;
        Label* title;

        void refreshView()
        {
            this->generateTexture();
            this->propagateParentAbsPos();
            for(GraphicElement* el: this->getElements())
                el->generateTexture();
        }

    public:
        Popup(std::string title="", sf::Vector2i pos = sf::Vector2i(POPUP_MARGIN, POPUP_MARGIN), sf::Vector2i size = sf::Vector2i(CONSTANT::WINDOW_WIDTH-POPUP_MARGIN*2, CONSTANT::WINDOW_HEIGHT-POPUP_MARGIN*2))
        {
            this->setDimensions(pos.x-SHADOW_SIZE, pos.y-SHADOW_SIZE, size.x+SHADOW_SIZE*2, size.y+SHADOW_SIZE*2);
            this->setReceiveEvents(true);
            this->setClearColor(CONSTANT::COLOR_BACK);
            this->generateTexture();

            this->closeBtn = new Button("X", sf::Vector2i(this->getWinSize().x-HEADER_SIZE, -HEADER_SIZE), sf::Vector2i(HEADER_SIZE, HEADER_SIZE), 20, CONSTANT::COLOR_CLEAR, CONSTANT::COLOR_RED_LIGHT);
            this->title = new Label(title, sf::Vector2i(HEADER_SIZE, -HEADER_SIZE), sf::Vector2i(this->getWinSize().x-HEADER_SIZE*2, HEADER_SIZE), 20, Label::CENTER, CONSTANT::COLOR_FORE, CONSTANT::COLOR_CLEAR);
            this->addComponent(new Label("", sf::Vector2i(0, -HEADER_SIZE), sf::Vector2i(HEADER_SIZE, HEADER_SIZE), 20, Label::CENTER, CONSTANT::COLOR_FORE, CONSTANT::COLOR_CLEAR));
            this->addComponent(this->title);
            this->addComponent(this->closeBtn);
            this->propagateParentAbsPos();
        }

        void addComponent(GraphicElement* el)
        {
            el->setPosition(el->getPosition() + sf::Vector2i(SHADOW_SIZE, HEADER_SIZE+SHADOW_SIZE));
            this->addElement(el);
            this->refreshView();
        }

        void removeComponent(GraphicElement* el)
        {
            this->removeElement(el);
            this->refreshView();
        }

        void clearComponents()
        {
            this->clearElements();
            this->refreshView();
        }

        void removeComponent(int index)
        {
            this->removeElement(index);
            this->refreshView();
        }

        void generateTexture()
        {
            this->renderTexture.clear(sf::Color(0, 0, 0, 64));
            sf::RectangleShape rect(sf::Vector2f(this->getSize().x-SHADOW_SIZE*2, this->getSize().y-SHADOW_SIZE*2));
            rect.setPosition(sf::Vector2f(SHADOW_SIZE, SHADOW_SIZE));
            rect.setFillColor(this->getClearColor());
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

        void setTitle(std::string title)
        {
            this->title->setText(title);
            this->generateTexture();
        }

        sf::Vector2i getWinSize()
        {
            return this->getSize() - sf::Vector2i(SHADOW_SIZE*2, HEADER_SIZE+SHADOW_SIZE*2);
        }

        std::string getTitle()
        {
            return this->title->getText();
        }

        Button* getCloseButton()
        {
            return this->closeBtn;
        }

        sf::Sprite getSprite(float dt)
        {
            return this->sprite;
        }

        ~Popup()
        {

        }

    };
}