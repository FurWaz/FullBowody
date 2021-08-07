#pragma once
#include "./UI/window.hpp"
#include "./UI/button.hpp"
#include "./UI/image.hpp"
#include "./UI/fpsCounter.hpp"
#include "./UI/label.hpp"
#include "./UI/input.hpp"
#include "./UI/cameraView.hpp"
#include "./UI/view.hpp"
#include "./UI/list.hpp"
#include "./UI/menuBar.hpp"

#include "./engine/camera.hpp"
#include "./engine/cameraManager.hpp"
#include "./engine/bodypos.hpp"

using namespace owo;

namespace SceneManager
{
    Window* win;
    MenuBar* menuBar;

    std::vector<Camera*> cameras;
    BodyPos bp;
    List* camList;

    void _populate_camlist();
    void _add_camera()
    {
        cameras.push_back(new Camera());
        _populate_camlist();
    }

    void _rem_camera()
    {
        if (cameras.size() == 0) return;
        cameras.erase(cameras.end()-1);
        _populate_camlist();
    }

    void _populate_camlist()
    {
        camList->clearComponents();
        for (Camera* cam: cameras)
        {
            CameraView* camView = new CameraView(cam, sf::Vector2i(10, 10), sf::Vector2i(camList->getSize().x-20, 500));
            camList->addComponent(camView);
        }
        Button* b = new Button("Ajouter", sf::Vector2i(10, 20), sf::Vector2i(100, 50));
        b->setCallback(_add_camera);
        camList->addComponent(b);
        b = new Button("Supprimer", sf::Vector2i(10, 20), sf::Vector2i(100, 50));
        b->setCallback(_rem_camera);
        camList->addComponent(b);
    }

    void setWindow(Window* window)
    {
        win = window;
    }

    void GenerateDefaultScene()
    {
        win->clearElements();
        win->addElement(menuBar);
        menuBar->setSelectedButton(0);

        camList = new List(sf::Vector2i(0, MenuBar::MENUBAR_HEIGHT), sf::Vector2i(CONSTANT::WINDOW_WIDTH*0.6, CONSTANT::WINDOW_HEIGHT-MenuBar::MENUBAR_HEIGHT), CONSTANT::COLOR_CLEAR);
        win->addElement(camList);
        _populate_camlist();

        View* v = new View(sf::Vector2i(CONSTANT::WINDOW_WIDTH*0.6, MenuBar::MENUBAR_HEIGHT), sf::Vector2i(CONSTANT::WINDOW_WIDTH*0.4, CONSTANT::WINDOW_HEIGHT-MenuBar::MENUBAR_HEIGHT), CONSTANT::COLOR_BACK);
        v->setBodyPos(&bp);
        win->addElement(v);
    }

    void GenerateOptionsScene()
    {
        win->clearElements();
        win->addElement(menuBar);
        menuBar->setSelectedButton(1);
    }

    void GenerateTestScene()
    {
        win->clearElements();
        List* l = new List(sf::Vector2i(20, 20), sf::Vector2i(150, CONSTANT::WINDOW_HEIGHT-40), CONSTANT::COLOR_BACK);
        Button* b1 = new Button("First", sf::Vector2i(10, 10), sf::Vector2i(100, 50), 20, CONSTANT::COLOR_RED_LIGHT, CONSTANT::COLOR_CLEAR);
        Button* b2 = new Button("Second", sf::Vector2i(10, 10), sf::Vector2i(100, 50), 20, CONSTANT::COLOR_RED_LIGHT, CONSTANT::COLOR_CLEAR);
        Button* b3 = new Button("Third", sf::Vector2i(10, 10), sf::Vector2i(100, 50), 20, CONSTANT::COLOR_RED_LIGHT, CONSTANT::COLOR_CLEAR);
        l->addComponent(b1);
        l->addComponent(b2);
        l->addComponent(b3);
        l->propagateParentAbsPos();
        win->addElement(l);
    }

    void GenerateExtensionsScene()
    {
        win->clearElements();
        win->addElement(menuBar);
        menuBar->setSelectedButton(2);
    }

    void GenerateAboutScene()
    {
        win->clearElements();
        win->addElement(menuBar);
        menuBar->setSelectedButton(3);
    }

    void init()
    {
        menuBar = new MenuBar();

        Button* btn;
        btn = new Button("Default", sf::Vector2i(0, 0), sf::Vector2i(150, MenuBar::MENUBAR_HEIGHT), 20, CONSTANT::COLOR_BLACK_DARKER);
        btn->setCallback(GenerateDefaultScene);
        menuBar->addComponent(btn);
        btn = new Button("Options", sf::Vector2i(0, 0), sf::Vector2i(150, MenuBar::MENUBAR_HEIGHT), 20, CONSTANT::COLOR_BLACK_DARKER);
        btn->setCallback(GenerateOptionsScene);
        menuBar->addComponent(btn);
        btn = new Button("Extensions", sf::Vector2i(0, 0), sf::Vector2i(150, MenuBar::MENUBAR_HEIGHT), 20, CONSTANT::COLOR_BLACK_DARKER);
        btn->setCallback(GenerateExtensionsScene);
        menuBar->addComponent(btn);
        btn = new Button("About", sf::Vector2i(0, 0), sf::Vector2i(150, MenuBar::MENUBAR_HEIGHT), 20, CONSTANT::COLOR_BLACK_DARKER);
        btn->setCallback(GenerateAboutScene);
        menuBar->addComponent(btn);
    }
}