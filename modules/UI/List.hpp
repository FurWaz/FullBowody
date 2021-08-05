#pragma once
#include "./graphicElement.hpp"
#include "../constants.hpp"
#include "./label.hpp"

namespace owo
{
    class List : public virtual GraphicElement
    {
    private:
        std::vector<GraphicElement*> elements;

        void init()
        {
            this->sprite.setTexture(this->renderTexture.getTexture());
            this->setReceiveEvents(true);
        }

    public:
        List()
        {
            this->setDimensions(0, 0, 200, 200);
            this->setClearColor(CONSTANT::COLOR_BACK);
            this->init();
        }

        List(sf::Vector2i pos, sf::Vector2i size, sf::Color clearColor)
        {
            this->setDimensions(pos.x, pos.y, size.x, size.y);
            this->setClearColor(clearColor);
            this->init();
        }

        void addComponent(GraphicElement* el)
        {
            el->setPosition(el->getPosition() + sf::Vector2i(0, this->calculateElementsSize().y));
            this->addElement(el);
            this->onScroll(0);
        }

        void removeComponent(GraphicElement* el)
        {
            this->removeElement(el);
            this->onScroll(0);
        }

        void removeComponent(int index)
        {
            this->removeElement(index);
            this->onScroll(0);
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
            sf::Vector2i dtPos(0, delta/2);
            this->generateTexture();
            for(GraphicElement* el: this->getElements())
            {
                el->setPosition(el->getPosition() + dtPos);
                el->generateTexture();
            }
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
            dimensions.y = child->getPosition().y + child->getSize().y;
            return dimensions;
        }

        ~List()
        {

        }

    };
}