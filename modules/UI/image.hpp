#pragma once
#include "../constants.hpp"
#include "graphicElement.hpp"

namespace owo {
    class Image : public virtual GraphicElement
    {
    private:
        sf::Image img;
        std::string path;

        void generateTexture()
        {
            sf::Vector2u imgSize = this->img.getSize();
            sf::Texture tex;
            tex.loadFromImage(this->img);
            tex.setSmooth(true);
            sf::Sprite s(tex);

            this->renderTexture.create(this->dimensions.width, this->dimensions.height);
            this->renderTexture.clear(CONST::COLOR_TRANS);

            float scaleFactor = 0;
            if ((this->dimensions.width / (float)this->dimensions.height) > (imgSize.x / (float)imgSize.y)) // scale on height
                scaleFactor = this->dimensions.height / (float) imgSize.y;
            else
                scaleFactor = this->dimensions.width / (float) imgSize.x;
                
            s.setScale(scaleFactor, scaleFactor);
            s.setPosition((this->dimensions.width-imgSize.x*scaleFactor)/2, (this->dimensions.height-imgSize.y*scaleFactor)/2);
            this->renderTexture.draw(s);
            this->renderTexture.display();
            
            this->sprite.setPosition(this->dimensions.left, this->dimensions.top);
            this->sprite.setTexture(this->renderTexture.getTexture(), true);
        }

    public:
        Image()
        {
            setDimensions(0, 0, 300, 300);
            this->img.create(300, 300, sf::Color::Black);
            this->path = "";
            generateTexture();
        }

        Image(int x, int y, int width, int height)
        {
            setDimensions(x, y, width, height);
            generateTexture();
        }

        Image(std::string path)
        {
            sf::Image img;
            this->path = path;
            if (!this->img.loadFromFile(path))
            {
                std::cout << "Couldn't load image at " << path << std::endl;
                this->img.create(300, 300, sf::Color::Black);
            }
            setDimensions(0, 0, 300, 300);
            generateTexture();
        }

        Image(std::string path, int x, int y, int width, int height)
        {
            sf::Image img;
            this->path = path;
            if (!this->img.loadFromFile(path))
            {
                std::cout << "Couldn't load image at " << path << std::endl;
                this->img.create(300, 300, sf::Color::Black);
            }
            setDimensions(x, y, width, height);
            generateTexture();
        }

        void resizeTo(int width, int height)
        {
            this->dimensions.width = width;
            this->dimensions.height = height;
            generateTexture();
        }

        void fromArray(const sf::Uint8 *pixels, sf::Vector2u dims)
        {
            this->img.create(dims.x, dims.y, pixels);
            this->generateTexture();
        }

        void black(sf::Vector2u dims)
        {
            this->img.create(dims.x, dims.y, sf::Color(0, 0, 0));
            this->generateTexture();
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

        ~Image()
        {

        }

    };
}