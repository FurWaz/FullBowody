#pragma once
#include "graphicElement.hpp"
#include "../constants.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <thread>

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
        
        std::vector<GraphicElement*> elements;
        GraphicElement* focused = nullptr;
        bool shouldUpdate = false;

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
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
        }

        void setWindowConstants()
        {
            CONSTANT::WINDOW_HEIGHT = this->getHeight();
            CONSTANT::WINDOW_WIDTH = this->getWidth();
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
            for (GraphicElement* el: this->elements)
                this->screen.draw(el->getSprite(this->refreshDelta));
            screen.display();
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

            int key;
            switch (event.type)
            {
                case sf::Event::Closed:
                    this->close();
                    break;

                // case sf::Event::Resized:
                //     this->setSize(event.size.width, event.size.height);
                //     break;

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
            
            for (std::vector<GraphicElement*>::iterator i = this->elements.end()-1; i >= this->elements.begin(); i--)
            {
                GraphicElement* el = (*i);
                if (el->doesReceiveEvents())
                {
                    if (el->collides(this->mousePos))
                    {
                        if (!el->isHovered())
                            el->onHover(true);
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
            this->elements.push_back(el);
            for(GraphicElement* element: el->getElements())
                this->addElement(element);
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
            for(GraphicElement* el: this->elements.at(index)->getElements())
                this->removeElement(el);
            this->elements.erase(this->elements.begin()+index);
        }

        /**
         * @brief Removes all the GraphicElements from the window
         */
        void clearElements()
        {
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