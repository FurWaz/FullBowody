#include "graphicElement.hpp"

class Template : public virtual GraphicElement
{
private:

public:
    Template()
    {
        
    }

    void onClick(bool clicked)
    {
        this->clicked = clicked;
    }

    void onHover(bool hovered)
    {
        this->hovered = hovered;
    }

    void update(sf::Vector2i mousePos)
    {
        
    }

    sf::Sprite getSprite()
    {
        return this->sprite;
    }

    ~Template()
    {

    }
};