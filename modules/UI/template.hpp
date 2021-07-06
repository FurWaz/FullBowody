#include "constants.hpp"
#include "graphicElement.hpp"

class Template : public virtual GraphicElement
{
private:

public:
    Template()
    {
        
    }

    void onClick(int btn, bool clicked)
    {
        this->clicked = clicked;
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

    sf::Sprite getSprite()
    {
        return this->sprite;
    }

    ~Template()
    {

    }

};