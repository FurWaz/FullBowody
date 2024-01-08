#pragma once
#include "../../constants.hpp"
#include "./graphicElement.hpp"

namespace owo
{
    class Image : public virtual GraphicElement
    {
    private:
        sf::Image img;
        std::string path;
        bool shouldUpdate;
        bool flip;

        sf::Sprite imgSprite;
        float scaleFactor;

        float blinkDelta;

        void generateTexture()
        {
            this->shouldUpdate = false;
            sf::Vector2u imgSize = this->img.getSize();
            sf::Texture tex;
            tex.loadFromImage(this->img);
            tex.setSmooth(true);
            imgSprite = sf::Sprite(tex);

            this->renderTexture.clear(this->clearColor);
            if ((this->dimensions.width / (float)this->dimensions.height) > (imgSize.x / (float)imgSize.y))
                this->scaleFactor = this->dimensions.height / (float) imgSize.y;
            else
                this->scaleFactor = this->dimensions.width / (float) imgSize.x;
                
            if (this->getFlipped())
            {
                imgSprite.setScale(-scaleFactor, scaleFactor);
                imgSprite.setPosition((this->dimensions.width+imgSize.x*scaleFactor)/2, (this->dimensions.height-imgSize.y*scaleFactor)/2);
            } else
            {
                imgSprite.setScale(scaleFactor, scaleFactor);
                imgSprite.setPosition((this->dimensions.width-imgSize.x*scaleFactor)/2, (this->dimensions.height-imgSize.y*scaleFactor)/2);
            }
            
            this->renderTexture.draw(imgSprite);
            this->renderTexture.display();
            
            this->sprite.setPosition(this->dimensions.left, this->dimensions.top);
            this->sprite.setTexture(this->renderTexture.getTexture(), true);
        }

        void updateTexture()
        {
            sf::Texture tex;
            tex.loadFromImage(this->img);
            tex.setSmooth(true);
            imgSprite.setTexture(tex);
            imgSprite.setColor(sf::Color(255, 255, 255));
            if (this->blinkDelta != 0)
                imgSprite.setColor(sf::Color(this->blinkDelta*255, this->blinkDelta*255, this->blinkDelta*255));
            this->renderTexture.draw(imgSprite);
            this->renderTexture.display();
            this->sprite.setTexture(this->renderTexture.getTexture());
        }

        void init()
        {
            this->setFlipped(false);
            this->blinkDelta = 0;
        }

    public:
        Image()
        {
            this->img.create(300, 300, sf::Color::Black);
            this->setClearColor(CONSTANT::COLOR_BACK);
            this->path = "";
            this->setDimensions(0, 0, 300, 300);
            this->init();
            this->generateTexture();
        }

        Image(sf::Vector2i pos, sf::Vector2i size, sf::Color clearColor = CONSTANT::COLOR_BACK)
        {
            this->img.create(300, 300, sf::Color::Black);
            this->setClearColor(clearColor);
            this->path = "";
            this->setDimensions(pos.x, pos.y, size.x, size.y);
            this->init();
            this->generateTexture();
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
            this->setDimensions(0, 0, 300, 300);
            this->init();
            this->generateTexture();
        }

        Image(std::string path, sf::Vector2i pos, sf::Vector2i size, sf::Color clearColor = CONSTANT::COLOR_BACK)
        {
            sf::Image img;
            this->path = path;
            if (!this->img.loadFromFile(path))
            {
                std::cout << "Couldn't load image at " << path << std::endl;
                this->img.create(300, 300, sf::Color::Black);
            }
            this->setClearColor(this->clearColor);
            this->setDimensions(pos.x, pos.y, size.x, size.y);
            this->init();
            this->generateTexture();
        }

        void resizeTo(sf::Vector2i size)
        {
            this->setSize(size);
            generateTexture();
        }

        void fromArray(const sf::Uint8 *pixels, sf::Vector2u dims)
        {
            bool shouldGenerate = false;
            if (dims != this->img.getSize())
                shouldGenerate = true;
            this->img.create(dims.x, dims.y, pixels);
            shouldGenerate? this->generateTexture() : this->updateTexture();
        }

        void black(sf::Vector2u dims)
        {
            bool shouldGenerate = false;
            if (dims != this->img.getSize())
                shouldGenerate = true;
            this->img.create(dims.x, dims.y, sf::Color(0, 0, 0, 0));
            shouldGenerate? this->generateTexture() : this->updateTexture();
        }

        void blink()
        {
            this->blinkDelta = 0.2;
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

        void setFlipped(bool state)
        {
            this->flip = state;
            this->generateTexture();
        }

        bool getFlipped()
        {
            return this->flip;
        }

        sf::Sprite getSprite(float dt)
        {
            this->blinkDelta = std::max(this->blinkDelta-dt, 0.f);
            if (this->blinkDelta != 0) this->updateTexture();
            return this->sprite;
        }

        ~Image()
        {

        }

    };
}