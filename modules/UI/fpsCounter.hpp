#include "constants.hpp"
#include "graphicElement.hpp"

class FPSCounter : public virtual GraphicElement
{
private:

public:
    FPSCounter()
    {
        setDimensions(0, 0, 200, 50);
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

    sf::Sprite getSprite(float dt)
    {
        std::string txt = std::to_string((int)(1 / dt)) + " FPS";

        this->renderTexture.create(300, 300);
        this->renderTexture.clear(sf::Color::Red);

        sf::Text text("yay", CONST::FONT, 20);
        text.setFillColor(sf::Color::White);
        this->renderTexture.draw(text);
        this->renderTexture.display();

        this->sprite.setTexture(this->renderTexture.getTexture());
        this->sprite.setPosition(sf::Vector2f(20, 20));
        return this->sprite;
    }

    ~FPSCounter()
    {
        
    }

};