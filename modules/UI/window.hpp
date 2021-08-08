#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <thread>
#include "../constants.hpp"
#include "../engine/bodypos.hpp"
#include "./essentials/graphicElement.hpp"

namespace owo
{
    class Window
    {
    private:
        sf::RenderWindow screen;
        sf::ContextSettings context;
        sf::Vector2i size;
        sf::Event event;
        sf::Vector2i mousePos;
        sf::Clock updateClock;
        sf::Clock refreshClock;

        std::thread updateThread;
        float refreshDelta;
        float updateDelta;
        
        std::vector<GraphicElement*> graphicElements, elements;
        GraphicElement* focused = nullptr;
        bool shouldUpdate = false;
        bool showHitboxes = false;

        BodyPos* bodyPos;

        /**
         * @brief Update the window's UI elements
         */
        void _update()
        {
            while (this->shouldUpdate)
            {
                this->updateDelta = this->updateClock.restart().asSeconds();
                for (GraphicElement* el: this->elements)
                    el->update(this->updateDelta, this->mousePos);
                bodyPos->update(this->updateDelta);
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
        }

        void setWindowConstants()
        {
            CONSTANT::WINDOW_HEIGHT = this->getHeight();
            CONSTANT::WINDOW_WIDTH = this->getWidth();
        }

        void drawElement(GraphicElement* el, sf::RenderTexture* tex)
        {
            sf::RenderTexture* pTex = el->getRenderTexture();
            for (GraphicElement* child: el->getElements())
                this->drawElement(child, pTex);
            tex->draw(el->getSprite(this->refreshDelta));
        }

        void drawElement(GraphicElement* el, sf::RenderWindow* tex)
        {
            sf::RenderTexture* pTex = el->getRenderTexture();
            for (GraphicElement* child: el->getElements())
                this->drawElement(child, pTex);
            tex->draw(el->getSprite(this->refreshDelta));
        }

        void addPhysicElement(GraphicElement* el)
        {
            this->elements.push_back(el);
            for (GraphicElement* child: el->getElements())
                this->addPhysicElement(child);
        }

    public:
        /**
         * @brief Construct a new Window object
         */
        Window()
        {
            
        }

        /**
         * @brief Construct a new Window object
         * @param name name of the window
         * @param width width of the window (in pixels)
         * @param height height of the window (in pixels)
         */
        Window(std::string name, int width, int height)
        {
            this->screen.create(sf::VideoMode(width, height), name, sf::Style::Default, this->context);
            this->setSize(width, height);
            this->updateClock.restart();
            this->refreshClock.restart();
            this->screen.setVerticalSyncEnabled(true);
        }

        /**
         * @brief Draw the window's UI elements
         */
        void refresh()
        {
            this->refreshDelta = this->refreshClock.restart().asSeconds();
            screen.clear(CONSTANT::COLOR_CLEAR);
            int index = 0;
            for (GraphicElement* el: this->graphicElements)
                this->drawElement(el, &this->screen);
                
            if (this->showHitboxes)
            {
                int index = 0;
                int max = this->elements.size();
                for(GraphicElement* el: this->elements)
                {
                    sf::RectangleShape rect;
                    rect.setPosition(el->getAbsoluteDimensions().left, el->getAbsoluteDimensions().top);
                    rect.setSize(sf::Vector2f(el->getAbsoluteDimensions().width, el->getAbsoluteDimensions().height));
                    rect.setOutlineThickness(2);
                    rect.setFillColor(CONSTANT::COLOR_TRANS);
                    rect.setOutlineColor(sf::Color(index*255/max, 0, (255-(index*255/max))));
                    this->screen.draw(rect);
                    index++;
                }
            }
            this->screen.display();
        }

        void startUpdating()
        {
            shouldUpdate = true;
            this->updateThread = std::thread(&Window::_update, this);
        }

        void stopUpdating()
        {
            shouldUpdate = false;
            this->updateThread.join();
        }

        /**
         * @brief Processes the window events
         */
        void processEvents()
        {
            if (!this->screen.pollEvent(this->event)) return;

            this->elements.clear();
            for (GraphicElement* el: this->graphicElements)
                this->addPhysicElement(el);

            int key;
            switch (event.type)
            {
                case sf::Event::Closed:
                    this->close();
                    break;

                case sf::Event::MouseMoved:
                    this->mousePos = sf::Vector2i(event.mouseMove.x, event.mouseMove.y);
                    break;

                case sf::Event::MouseButtonPressed:
                    this->focused = nullptr;
                    for (std::vector<GraphicElement*>::iterator i = this->elements.end()-1; i >= this->elements.begin(); i--)
                    {
                        GraphicElement* el = (*i);
                        if (!el->doesReceiveEvents()) continue;
                        if (el->isHovered())
                        {
                            el->onClick(event.mouseButton.button, true);
                            this->focused = el;
                            if (!el->isFocused()) el->onFocus(true);
                            break;
                        } else
                            if (el->isFocused()) el->onFocus(false);
                    }
                    break;

                case sf::Event::MouseButtonReleased:
                    for (std::vector<GraphicElement*>::iterator i = this->elements.end()-1; i >= this->elements.begin(); i--)
                    {
                        GraphicElement* el = (*i);
                        if (!el->doesReceiveEvents() || !el->isHovered()) continue;
                        el->onClick(event.mouseButton.button, false);
                    }
                    break;

                case sf::Event::MouseWheelScrolled:
                    for (std::vector<GraphicElement*>::iterator i = this->elements.end()-1; i >= this->elements.begin(); i--)
                    {
                        GraphicElement* el = (*i);
                        if (!el->doesReceiveEvents() || !el->isHovered()) continue;
                        el->onScroll(event.mouseWheelScroll.delta*100);
                    }
                    break;

                case sf::Event::KeyPressed:
                    if (this->focused == nullptr) return;
                    key = event.key.code;
                    this->screen.pollEvent(this->event);
                    if (event.type == sf::Event::TextEntered)
                        this->focused->onKey(key, event.text.unicode, true);
                    else this->focused->onKey(key, CONSTANT::NO_CHAR, true);
                    break;

                case sf::Event::KeyReleased:
                    if (this->focused == nullptr) return;
                    key = event.key.code;
                    this->screen.pollEvent(this->event);
                    if (event.type == sf::Event::TextEntered)
                        this->focused->onKey(key, event.text.unicode, false);
                    else this->focused->onKey(key, CONSTANT::NO_CHAR, false);
                    break;
                    
                default:
                    break;
            }
            
            bool canHoverTrue = true;
            for (std::vector<GraphicElement*>::iterator i = this->elements.end()-1; i >= this->elements.begin(); i--)
            {
                GraphicElement* el = (*i);
                if (el->doesReceiveEvents())
                {
                    if (el->collides(this->mousePos))
                    {
                        if (!el->isHovered() && canHoverTrue)
                            el->onHover(true);
                        canHoverTrue = false;
                    }
                    else
                    {
                        if (el->isHovered())
                            el->onHover(false);
                    }
                }
            }
        }

        /**
         * @brief Add a GraphicElement to the UI
         * @param el The GraphicElement to add
         */
        void addElement(GraphicElement* el)
        {
            this->graphicElements.push_back(el);
        }

        /**
         * @brief Remove a GraphicElement from the UI
         * @param el The GraphicElement to remove
         */
        void removeElement(GraphicElement* el)
        {
            int index = -1;
            int counter = 0;
            for(GraphicElement* element: this->graphicElements)
            {
                if (element == el)
                {
                    index = counter;
                    break;
                }
                counter++;
            }
            if (index >= 0)
                this->removeElement(index);
        }

        /**
         * @brief Remove a GraphicElement from the UI
         * @param index The index of the element to remove
         */
        void removeElement(int index)
        {
            this->graphicElements.erase(this->graphicElements.begin()+index);
        }

        /**
         * @brief Removes all the GraphicElements from the window
         */
        void clearElements()
        {
            this->graphicElements.clear();
            this->elements.clear();
        }

        /**
         * @brief Returns if the window is open 
         * @return The window is open or closed
         */
        bool isOpen()
        {
            return this->screen.isOpen();
        }

        /**
         * @brief Close the window
         */
        void close()
        {
            this->shouldUpdate = false;
            this->screen.close();
        }

        void setBodyPos(BodyPos* bp)
        {
            this->bodyPos = bp;
        }

        void setShowHitBoxes(bool state)
        {
            this->showHitboxes = state;
        }

        bool doesShowhitboxes()
        {
            return this->showHitboxes;
        }

        int getWidth()
        {
            return this->size.x;
        }

        void setSize(int w, int h)
        {
            this->size = sf::Vector2i(w, h);
            this->screen.setView(sf::View(sf::FloatRect(0, 0, w, h)));
            this->setWindowConstants();
        }

        int getHeight()
        {
            return this->size.y;
        }

        float getDeltaTime()
        {
            return this->refreshDelta;
        }

        sf::Vector2i getMousePos()
        {
            return this->mousePos;
        }

        /**
         * @brief Destroy the Window object
         */
        ~Window()
        {
            if (this->screen.isOpen())
                this->screen.close();
        }
    };
}