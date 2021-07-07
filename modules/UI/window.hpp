#pragma once
#include "graphicElement.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <thread>

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
            float dt = this->updateClock.restart().asSeconds();
            for (GraphicElement* el: this->elements)
                el->update(dt, this->mousePos);
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
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
        this->size = sf::Vector2i(width, height);
        this->updateClock.restart();
        this->refreshClock.restart();
        this->screen.setVerticalSyncEnabled(true);
    }

    /**
     * @brief Draw the window's UI elements
     */
    void refresh()
    {
        float dt = this->refreshClock.restart().asSeconds();
        screen.clear(sf::Color(30, 20, 10));
        int index = 0;
        for (GraphicElement* el: this->elements)
            this->screen.draw(el->getSprite(dt));
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
            this->screen.close();
            break;
        case sf::Event::MouseMoved:
            this->mousePos = sf::Vector2i(event.mouseMove.x, event.mouseMove.y);
            break;

        case sf::Event::MouseButtonPressed:
        this->focused = nullptr;
            for (GraphicElement* el: this->elements)
                if (el->isHovered())
                {
                    el->onClick(event.mouseButton.button, true);
                    this->focused = el;
                    if (!el->isFocused()) el->onFocus(true);
                } else
                    if (el->isFocused()) el->onFocus(false);
            break;
        case sf::Event::MouseButtonReleased:
            for (GraphicElement* el: this->elements)
                if (el->isHovered())
                    el->onClick(event.mouseButton.button, false);
            break;
        case sf::Event::MouseWheelScrolled:
            for (GraphicElement* el: this->elements)
                if (el->isHovered())
                    el->onScroll(event.mouseWheelScroll.delta*100);
            break;
        case sf::Event::KeyPressed:
            if (this->focused == nullptr) return;
            key = event.key.code;
            this->screen.pollEvent(this->event);
            if (event.type == sf::Event::TextEntered)
                this->focused->onKey(key, event.text.unicode, true);
            else this->focused->onKey(key, CONST::NO_CHAR, true);
            break;
        case sf::Event::KeyReleased:
            if (this->focused == nullptr) return;
            key = event.key.code;
            this->screen.pollEvent(this->event);
            if (event.type == sf::Event::TextEntered)
                this->focused->onKey(key, event.text.unicode, false);
            else this->focused->onKey(key, CONST::NO_CHAR, false);
            break;
        default:
            break;
        }
        
        for (GraphicElement* el: this->elements)
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
        this->screen.close();
    }

    int getWidth()
    {
        return this->size.x;
    }

    int getHeight()
    {
        return this->size.y;
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