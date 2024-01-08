#pragma once
#include <SFML/Graphics.hpp>
#include <vector>


namespace owo
{
    class GraphicElement
    {
    private:
        std::vector<GraphicElement*> elements;

    protected:
        sf::Sprite sprite;
        sf::RenderTexture renderTexture;
        sf::IntRect dimensions, absDims, parentAbsDims;
        sf::Color clearColor;

        bool hovered;
        bool clicked;
        bool focused;
        bool receiveEvents;

        void calculateAbsDims()
        {
            this->absDims = sf::IntRect(
                this->dimensions.left + this->parentAbsDims.left,
                this->dimensions.top + this->parentAbsDims.top,
                this->dimensions.width,
                this->dimensions.height
            );
        }

    public:
        /**
         * @brief Construct a new Graphic Element object
         */
        GraphicElement()
        {
            this->clearColor = CONSTANT::COLOR_TRANS;
            this->hovered = false;
            this->clicked = false;
            this->focused = false;
            this->receiveEvents = false;
            this->parentAbsDims = sf::IntRect(0, 0, CONSTANT::WINDOW_WIDTH, CONSTANT::WINDOW_HEIGHT);
            this->calculateAbsDims();
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
            this->dimensions = sf::IntRect(x, y, width, height);
            this->calculateAbsDims();
            this->propagateParentAbsPos();
            this->renderTexture.create(this->dimensions.width, this->dimensions.height);
            this->sprite.setPosition(this->dimensions.left, this->dimensions.top);
        }

        /**
         * @brief Returns the list of child elements of this element
         */
        std::vector<GraphicElement*> getElements()
        {
            return this->elements;
        }

        /**
         * @brief Add a GraphicElement to the UI
         * @param el The GraphicElement to add
         */
        void addElement(GraphicElement* el)
        {
            this->elements.push_back(el);
        }

        /**
         * @brief Remove a GraphicElement from the UI
         * @param el The GraphicElement to remove
         */
        void removeElement(GraphicElement* el)
        {
            int index = -1;
            int counter = 0;
            for(GraphicElement* element: this->elements)
            {
                if (element == el)
                {
                    index = counter;
                    break;
                }
                counter++;
            }
            this->removeElement(index);
        }

        /**
         * @brief Remove a GraphicElement from the UI
         * @param index The index of the element to remove
         */
        void removeElement(int index)
        {
            if (index >= this->elements.size() || index < 0)
                return;
            this->elements.erase(this->elements.begin()+index);
        }

        void clearElements()
        {
            if (this->elements.size() <= 0) return;
            for(int i = this->elements.size()-1; i >= 0; i--)
                this->removeElement(i);
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
         * @brief Redraws the element's texture
         */
        virtual void generateTexture() = 0;
        
        /**
         * @brief Returns if the position is in the element's hitbox
         * 
         * @param pos Position to test
         * @return If the position is in the element's hitbox
         */
        bool collides(sf::Vector2i pos)
        {
            return this->absDims.contains(pos);
        }

        /**
         * @brief Applies parent absolute position to all
         * child GraphicElement of this element
         */
        void propagateParentAbsPos()
        {
            for(GraphicElement* el: this->getElements())
                el->setParentAboluteDimensions(this->absDims);
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

        bool doesReceiveEvents()
        {
            return this->receiveEvents;
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

        void setReceiveEvents(bool state)
        {
            this->receiveEvents = state;
        }

        sf::Color getClearColor()
        {
            return this->clearColor;
        }

        sf::RenderTexture* getRenderTexture()
        {
            return &this->renderTexture;
        }

        sf::IntRect getDimensions()
        {
            return this->dimensions;
        }

        void setParentAboluteDimensions(sf::IntRect dims)
        {
            this->parentAbsDims = dims;
            this->calculateAbsDims();
            this->propagateParentAbsPos();
        }

        sf::IntRect getAbsoluteDimensions()
        {
            return this->absDims;
        }

        sf::Vector2i getPosition()
        {
            return sf::Vector2i(this->dimensions.left, this->dimensions.top);
        }

        sf::Vector2i getSize()
        {
            return sf::Vector2i(this->dimensions.width, this->dimensions.height);
        }

        virtual ~GraphicElement()
        {
            
        }
    };
}