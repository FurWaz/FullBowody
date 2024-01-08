#pragma once
#include "../../constants.hpp"
#include "./graphicElement.hpp"

namespace owo
{
    class List : public virtual GraphicElement
    {
    private:
        std::vector<GraphicElement*> elements;
        int scrollDelta;

        void init()
        {
            this->sprite.setTexture(this->renderTexture.getTexture());
            this->setReceiveEvents(true);
            this->generateTexture();
            this->scrollDelta = 0;
        }

        void refreshView()
        {
            this->generateTexture();
            for(GraphicElement* el: this->getElements())
                el->generateTexture();
            this->propagateParentAbsPos();
        }

    public:
        List()
        {
            this->setDimensions(0, 0, 200, 200);
            this->setClearColor(CONSTANT::COLOR_BACK);
            this->init();
        }

        List(sf::Vector2i pos, sf::Vector2i size, sf::Color clearColor = CONSTANT::COLOR_CLEAR)
        {
            this->setDimensions(pos.x, pos.y, size.x, size.y);
            this->setClearColor(clearColor);
            this->init();
        }

        void addComponent(GraphicElement* el)
        {
            el->setPosition(el->getPosition() + sf::Vector2i(0, this->calculateElementsSize().y + this->scrollDelta));
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
            int elemSize = this->calculateElementsSize().y;
            if (delta > 0 && this->scrollDelta+delta >= 0) delta = -this->scrollDelta;
            if (delta < 0 && elemSize+this->scrollDelta+delta+10 < this->dimensions.height) 
            {
                if (elemSize > this->dimensions.height)
                    delta = (this->dimensions.height-elemSize-10) - this->scrollDelta;
                else delta = 0;
            }

            sf::Vector2i dtPos(0, delta);
            this->generateTexture();
            for(GraphicElement* el: this->getElements())
            {
                el->setPosition(el->getPosition() + dtPos);
                el->generateTexture();
            }
            this->scrollDelta += delta;
        }

        void update(float dt, sf::Vector2i mousePos)
        {
            
        }

        sf::Sprite getSprite(float dt)
        {
            return this->sprite;
        }

        sf::Vector2i calculateElementsSize()
        {
            if (this->getElements().size() == 0)
                return sf::Vector2i();

            sf::Vector2i dimensions;
            GraphicElement* child = this->getElements().at(this->getElements().size()-1);
            for(GraphicElement* el: this->getElements())
                dimensions.x = std::max(dimensions.x,  el->getSize().x);
            dimensions.y = child->getPosition().y + child->getSize().y - this->scrollDelta;
            return dimensions;
        }

        ~List()
        {

        }

    };
}