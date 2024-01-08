#pragma once
#include "../../constants.hpp"
#include "./graphicElement.hpp"

namespace owo
{
    class Container : public virtual GraphicElement
    {
    private:
        std::vector<GraphicElement*> elements;

        void init()
        {
            this->setReceiveEvents(false);
            this->generateTexture();
        }

        void refreshView()
        {
            this->generateTexture();
            for(GraphicElement* el: this->getElements())
                el->generateTexture();
            this->propagateParentAbsPos();
        }

    public:
        Container()
        {
            this->setDimensions(0, 0, 200, 200);
            this->setClearColor(CONSTANT::COLOR_BACK);
            this->init();
        }

        Container(sf::Vector2i pos, sf::Vector2i size, sf::Color clearColor = CONSTANT::COLOR_CLEAR)
        {
            this->setDimensions(pos.x, pos.y, size.x, size.y);
            this->setClearColor(clearColor);
            this->init();
        }

        void addComponent(GraphicElement* el)
        {
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
            if (index < 0) return;
            this->removeElement(index);
            this->refreshView();
        }

        void clearComponents()
        {
            this->clearElements();
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

        sf::Sprite getSprite(float dt)
        {
            return this->sprite;
        }

        ~Container()
        {

        }

    };
}