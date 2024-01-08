#pragma once
#include "./constants.hpp"
#include "./UI/window.hpp"
#include "./UI/popup.hpp"
#include "./UI/essentials/graphicElement.hpp"
#include "./UI/essentials/callbackContainer.hpp"
#include "./UI/essentials/label.hpp"
#include "./UI/essentials/input.hpp"

using namespace owo;


namespace Files
{
    StringCallbackContainer* cont;
    Window* window;
    Input* input;
    Popup* popup;

    void removePopup()
    {
        window->removeElement(popup);
    }

    void _execute_callback()
    {
        cont->func(input->getText());
        removePopup();
    }

    void askForFile()
    {
        popup = new Popup("File name", sf::Vector2i(CONSTANT::WINDOW_WIDTH/2-200, CONSTANT::WINDOW_HEIGHT*0.25f), sf::Vector2i(400, 100));
        input = new Input("", sf::Vector2i(10, 10), sf::Vector2i(320, 40), 20, Input::LEFT, CONSTANT::COLOR_FORE, "Enter file name");
        input->setCallback(CONSTANT::VOIDCALLBACK_STRING);
        Button* btn = new Button("OK", sf::Vector2i(340, 10), sf::Vector2i(50, 40), 20);
        btn->setCallback(_execute_callback);
        popup->getCloseButton()->setCallback(removePopup);
        popup->addComponent(input);
        popup->addComponent(btn);
        window->addElement(popup);
        input->generateTexture();
        btn->generateTexture();
    }

    void setWindow(Window* win)
    {
        window = win;
    }

    template<class T> void setCallback(void (T::*callback)(std::string), T* instance)
    {
        cont = new TypedStringCallbackContainer<T>(callback, instance);
    }

    void setCallback(void (callback)(std::string))
    {
        cont = new VoidStringCallbackContainer(callback);
    }
}