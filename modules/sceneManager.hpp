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

    CameraManager camMan;

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
        bp.setCameras(cameras);
        camMan.setCameras(cameras);
        camList->clearComponents();
        for (Camera* cam: cameras)
        {
            CameraView* camView = new CameraView(cam, sf::Vector2i(10, 10), sf::Vector2i(camList->getSize().x-20, 400));
            camList->addComponent(camView);
        }
        Button* b = new Button("Add", sf::Vector2i(10, 20), sf::Vector2i(100, 50));
        b->setCallback(_add_camera);
        camList->addComponent(b);
        b = new Button("Remove", sf::Vector2i(120, -50), sf::Vector2i(100, 50));
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
        const int TITLE_SIZE = 30;

        win->addElement(new Title(sf::Vector2i(CONSTANT::WINDOW_WIDTH*0.2, MenuBar::MENUBAR_HEIGHT), sf::Vector2i(CONSTANT::WINDOW_WIDTH*0.2, TITLE_SIZE), "Cameras", 18, CONSTANT::COLOR_CLEAR));

        camList = new List(sf::Vector2i(0, MenuBar::MENUBAR_HEIGHT+TITLE_SIZE), sf::Vector2i(CONSTANT::WINDOW_WIDTH*0.6, CONSTANT::WINDOW_HEIGHT-MenuBar::MENUBAR_HEIGHT-TITLE_SIZE), CONSTANT::COLOR_CLEAR);
        win->addElement(camList);
        _populate_camlist();

        win->addElement(new Title(sf::Vector2i(CONSTANT::WINDOW_WIDTH*0.7, MenuBar::MENUBAR_HEIGHT), sf::Vector2i(CONSTANT::WINDOW_WIDTH*0.2, TITLE_SIZE), "3D View", 18, CONSTANT::COLOR_CLEAR));

        View* v = new View(sf::Vector2i(CONSTANT::WINDOW_WIDTH*0.6, MenuBar::MENUBAR_HEIGHT+TITLE_SIZE), sf::Vector2i(CONSTANT::WINDOW_WIDTH*0.4, CONSTANT::WINDOW_HEIGHT-MenuBar::MENUBAR_HEIGHT-TITLE_SIZE));
        v->setBodyPos(&bp);
        win->addElement(v);

        Label* separator = new Label("", sf::Vector2i(CONSTANT::WINDOW_WIDTH*0.6-2, MenuBar::MENUBAR_HEIGHT+TITLE_SIZE), sf::Vector2i(4, CONSTANT::WINDOW_HEIGHT-MenuBar::MENUBAR_HEIGHT-TITLE_SIZE), 16, Label::CENTER, CONSTANT::COLOR_BACK);
        win->addElement(separator);

        Button* trackBtn = new Button("Start tracking", sf::Vector2i(CONSTANT::WINDOW_WIDTH*0.725, CONSTANT::WINDOW_HEIGHT-90), sf::Vector2i(CONSTANT::WINDOW_WIDTH*0.15, 70), 20);
        camMan.attachButton(trackBtn);
        trackBtn->setCallback(&CameraManager::toogleCameraTracking, &camMan);
        win->addElement(trackBtn);
    }

    void GenerateOptionsScene()
    {
        win->clearElements();
        win->addElement(menuBar);
        menuBar->setSelectedButton(1);
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

        bp.setCameras(cameras);
    }
}