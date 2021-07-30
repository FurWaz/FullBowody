#pragma once
#include <SFML/Graphics.hpp>

namespace owo
{
    class GraphicElement
    {
    protected:
        sf::Sprite sprite;
        sf::RenderTexture renderTexture;
        sf::IntRect dimensions;
        sf::Color clearColor;

        bool hovered = false;
        bool clicked = false;
        bool focused = false;

    public:
        /**
         * @brief Construct a new Graphic Element object
         */
        GraphicElement()
        {
            this->clearColor = CONSTANT::COLOR_TRANS;
        }

        /**
         * @brief Set the Dimensions of the element
         * @param x position in the x axis
         * @param y position in the y axis
         * @param width width of the element
         * @param height height of the element
         */
        void setDimensions(int x, int y, int width, int height)
        {
            this->dimensions = sf::Rect<int>(x, y, width, height);
        }

        /**
         * @brief Returns the sprite correponding to the element's UI
         * @return The Sprite correponding to the element's UI
         */
        virtual sf::Sprite getSprite(float dt) = 0;

        /**
         * @brief OnHover event callback when mouse is hovering the object
         * @param hovering the mouse is hovering or not the object
         */
        virtual void onHover(bool hovered) = 0;

        /**
         * @brief OnFocus event callback when the element is in focus
         * @param hovering Is the element focused or not
         */
        virtual void onFocus(bool focused) = 0;

        /**
         * @brief OnClick event callback when mouse is pressed or released
         * @param code The mouse button code
         * @param pressed Is the mouse pressed or released
         */
        virtual void onClick(int btn, bool clicked) = 0;

        /**
         * @brief OnKey event callback when a keyboard key is pressed or released
         * @param code The pressed/released key code
         * @param pressed Is the key pressed or released
         */
        virtual void onKey(int key, char c, bool pressed) = 0;

        /**
         * @brief OnScroll event callback when a mouse scroll is made
         * @param delta The amount of scroll
         */
        virtual void onScroll(int delta) = 0;

        /**
         * @brief Updates the element's components
         * @param dt Update delta time
         * @param mousePos Current mouse position
         */
        virtual void update(float dt, sf::Vector2i mousePos) = 0;
        
        /**
         * @brief Returns if the position is in the element's hitbox
         * 
         * @param pos Position to test
         * @return If the position is in the element's hitbox
         */
        bool collides(sf::Vector2i pos)
        {
            return this->dimensions.contains(pos);
        }

        /**
         * @brief Returns if the element is hovered or not
         * @return Is the element hovered or not
         */
        bool isHovered()
        {
            return this->hovered;
        }

        /**
         * @brief Returns if the element is clicked or not
         * @return Is the element clicked or not
         */
        bool isClicked()
        {
            return this->clicked;
        }

        /**
         * @brief Returns if the element is focused or not
         * @return Is the element focused or not
         */
        bool isFocused()
        {
            return this->focused;
        }

        void setClearColor(sf::Color color)
        {
            this->clearColor = color;
        }

        void setDimensions(sf::IntRect dims)
        {
            this->setDimensions(dims.left, dims.top, dims.width, dims.height);
        }

        void setPosition(sf::Vector2i pos)
        {
            this->setDimensions(pos.x, pos.y, this->dimensions.width, this->dimensions.height);
        }

        void setSize(sf::Vector2i size)
        {
            this->setDimensions(this->dimensions.left, this->dimensions.top, size.x, size.y);
        }

        sf::Color getClearColor()
        {
            return this->clearColor;
        }

        sf::IntRect getDimensions()
        {
            return this->dimensions;
        }

        sf::Vector2i getPosition()
        {
            return sf::Vector2i(this->dimensions.left, this->dimensions.top);
        }

        sf::Vector2i getSize()
        {
            return sf::Vector2i(this->dimensions.width, this->dimensions.height);
        }

        ~GraphicElement()
        {

        }
    };
}