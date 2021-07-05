#pragma once
#include "graphicElement.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>

class Window
{
private:
    sf::RenderWindow screen;
    sf::ContextSettings context;
    sf::Vector2i size;
    sf::Event event;
    sf::Vector2i mousePos;
    
    std::vector<GraphicElement*> elements;
    GraphicElement* focused = nullptr;

public:
    /**
     * @brief Construct a new Window object
     */
    Window()
    {
        this->screen.create(sf::VideoMode(1280, 720), "Window");
        this->size = sf::Vector2i(1280, 720);
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
    }

    /**
     * @brief Update the window's UI elements
     */
    void update()
    {

    }

    /**
     * @brief Draw the window's UI elements
     */
    void refresh()
    {
        screen.clear(sf::Color::Black);
        for (GraphicElement* el: this->elements)
            this->screen.draw(el->getSprite());
        screen.display();
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
                }
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
                if (!el->isHovered()) el->onHover(true);
            }
            else
            {
                if (el->isHovered()) el->onHover(false);
            }
        }

        for (GraphicElement* el: this->elements)
            el->update(mousePos);
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

    /**
     * @brief Destroy the Window object
     */
    ~Window()
    {
        if (this->screen.isOpen())
            this->screen.close();
    }
};