#pragma once
#include "../constants.hpp"
#include "./essentials/graphicElement.hpp"
#include "./essentials/button.hpp"
#include "./essentials/label.hpp"

namespace owo
{
    class MenuBar : public virtual GraphicElement
    {
    private:
        Label* label;

        void refreshView()
        {
            this->propagateParentAbsPos();
            this->generateTexture();
            for(GraphicElement* el: this->getElements())
                el->generateTexture();
        }

    public:
        static const int MENUBAR_HEIGHT = 40;

        MenuBar()
        {
            this->setDimensions(0, 0, CONSTANT::WINDOW_WIDTH, MENUBAR_HEIGHT);
            this->setClearColor(CONSTANT::COLOR_BLACK_DARKER);
            this->setReceiveEvents(true);
            this->label = new Label("FullBowody", sf::Vector2i(CONSTANT::WINDOW_WIDTH-210, 0), sf::Vector2i(200, this->dimensions.height), 20, Label::RIGHT, CONSTANT::COLOR_BLACK_LIGHT, CONSTANT::COLOR_BLACK_DARKER);
            this->generateTexture();
        }

        void generateTexture()
        {
            this->renderTexture.clear(this->clearColor);
            this->renderTexture.draw(this->label->getSprite(0));
            this->renderTexture.display();
            this->sprite.setTexture(this->renderTexture.getTexture());
        }

        void addComponent(GraphicElement* el)
        {
            el->setPosition(el->getPosition()+sf::Vector2i(this->calculateElementsSize(), 0));
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

        void setSelectedButton(int index)
        {
            for(GraphicElement* el: this->getElements())
                dynamic_cast<Button*>(el)->setBackColor(this->getClearColor());
            dynamic_cast<Button*>(this->getElements().at(index))->setBackColor(CONSTANT::COLOR_CLEAR);
            this->refreshView();
        }
        
        int calculateElementsSize()
        {
            if (this->getElements().size() == 0)
                return 0;
            GraphicElement* el = this->getElements().at(this->getElements().size()-1);
            return el->getPosition().x + el->getSize().x;
        }

        sf::Sprite getSprite(float dt)
        {
            return this->sprite;
        }

        ~MenuBar()
        {

        }

    };
}