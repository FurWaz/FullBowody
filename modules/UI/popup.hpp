#pragma once
#include "../constants.hpp"
#include "./graphicElement.hpp"
#include "./button.hpp"
#include "./label.hpp"

namespace owo
{
    class Popup : public virtual GraphicElement
    {
    private:
        static const int POPUP_MARGIN = 50;
        static const int HEADER_SIZE = 40;
        Button* closeBtn;
        Label* title;

        void refreshView()
        {
            this->propagateParentAbsPos();
            this->generateTexture();
            for(GraphicElement* el: this->getElements())
                el->generateTexture();
        }

    public:
        Popup(std::string title="")
        {
            this->setDimensions(POPUP_MARGIN, POPUP_MARGIN, CONSTANT::WINDOW_WIDTH-POPUP_MARGIN*2, CONSTANT::WINDOW_HEIGHT-POPUP_MARGIN*2);
            this->setReceiveEvents(true);
            this->setClearColor(CONSTANT::COLOR_CLEAR);
            this->generateTexture();

            this->closeBtn = new Button("X", sf::Vector2i(this->getSize().x-HEADER_SIZE, 0), sf::Vector2i(HEADER_SIZE, HEADER_SIZE), 20, CONSTANT::COLOR_CLEAR, CONSTANT::COLOR_RED_LIGHT);
            this->title = new Label(title, sf::Vector2i(this->getSize().x/2-100, 0), sf::Vector2i(200, HEADER_SIZE), 20, Label::CENTER, CONSTANT::COLOR_FORE, CONSTANT::COLOR_CLEAR);
            this->addElement(this->closeBtn);
            this->addElement(this->title);
            this->propagateParentAbsPos();
        }

        void addComponent(GraphicElement* el)
        {
            el->setPosition(el->getPosition() + sf::Vector2i(0, HEADER_SIZE));
            this->addElement(el);
            this->refreshView();
        }

        void removeComponent(GraphicElement* el)
        {
            this->removeElement(el);
            this->refreshView();
        }

        void removeComponent(int index)
        {
            this->removeElement(index);
            this->refreshView();
        }

        void generateTexture()
        {
            this->renderTexture.clear(this->clearColor);
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