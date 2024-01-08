#pragma once
#include "./UI/window.hpp"
#include "./UI/essentials/button.hpp"
#include "./UI/essentials/image.hpp"
#include "./UI/essentials/fpsCounter.hpp"
#include "./UI/essentials/label.hpp"
#include "./UI/essentials/input.hpp"
#include "./UI/essentials/view.hpp"
#include "./UI/essentials/list.hpp"
#include "./UI/essentials/container.hpp"
#include "./UI/cameraView.hpp"
#include "./UI/menuBar.hpp"
#include "./UI/softwareInfoPanel.hpp"

#include "./engine/camera.hpp"
#include "./engine/cameraManager.hpp"
#include "./engine/bodypos.hpp"
#include "./engine/softwareConnection.hpp"

using namespace owo;

namespace SceneManager
{
    Window* win;

    std::vector<Camera*> cameras;
    List* camList;
    CameraManager camMan;
    List* extensionList;

    BodyPos bp;
    SoftwareConnection con;

    unsigned char CURRENT_SCENE = 0;
    const unsigned char DEFAULT_SCENE = 0;
    const unsigned char OPTIONS_SCENE = 1;
    const unsigned char EXTENSIONS_SCENE = 2;
    const unsigned char ABOUT_SCENE = 3;
    bool shouldUpdate = false;

    // Buttons callbacks
    void _add_camera();
    void _rem_camera();
    void _set_tracking_fps(std::string fps);
    void _set_camera_fps(std::string fps);
    void _populate_camlist(bool modified, bool added);
    void _check_for_new_ext();

    void setWindow(Window* window)
    {
        win = window;
    }

    void init()
    {
        bp.setSoftwareConnection(&con);
        con.startConnection();
        con.setExtensionCallback(_check_for_new_ext);
    }

    void update()
    {
        if (shouldUpdate)
        {
            if (CURRENT_SCENE == EXTENSIONS_SCENE)
            {
                extensionList->clearComponents();
                for (SoftwareInfo soft: con.getExtensions())
                    extensionList->addComponent(new SoftwareInfoPanel(sf::Vector2i(10, 10), sf::Vector2i(200, 100), soft));
            }
            shouldUpdate = false;
        }
    }

    void shutdown()
    {
        con.stopConnection();
    }

    GraphicElement* createMenuBar(int pos);

    void GenerateDefaultScene()
    {
        win->clearElements();
        win->addElement(createMenuBar(DEFAULT_SCENE));
        const int TITLE_SIZE = 30;

        win->addElement(new Title(sf::Vector2i(CONSTANT::WINDOW_WIDTH*0.2, MenuBar::MENUBAR_HEIGHT), sf::Vector2i(CONSTANT::WINDOW_WIDTH*0.2, TITLE_SIZE), "Cameras", 18, CONSTANT::COLOR_CLEAR));

        camList = new List(sf::Vector2i(0, MenuBar::MENUBAR_HEIGHT+TITLE_SIZE), sf::Vector2i(CONSTANT::WINDOW_WIDTH*0.6, CONSTANT::WINDOW_HEIGHT-MenuBar::MENUBAR_HEIGHT-TITLE_SIZE-60), CONSTANT::COLOR_CLEAR);
        win->addElement(camList);
        _populate_camlist(false, false);

        Button* b = new Button("Add", sf::Vector2i(10, CONSTANT::WINDOW_HEIGHT-50), sf::Vector2i(100, 40));
        b->setCallback(_add_camera);
        win->addElement(b);

        b = new Button("Remove", sf::Vector2i(120, CONSTANT::WINDOW_HEIGHT-50), sf::Vector2i(100, 40));
        b->setCallback(_rem_camera);
        win->addElement(b);

        win->addElement(new Title(sf::Vector2i(CONSTANT::WINDOW_WIDTH*0.7, MenuBar::MENUBAR_HEIGHT), sf::Vector2i(CONSTANT::WINDOW_WIDTH*0.2, TITLE_SIZE), "3D View", 18, CONSTANT::COLOR_CLEAR));

        View* v = new View(sf::Vector2i(CONSTANT::WINDOW_WIDTH*0.6, MenuBar::MENUBAR_HEIGHT+TITLE_SIZE), sf::Vector2i(CONSTANT::WINDOW_WIDTH*0.4, CONSTANT::WINDOW_HEIGHT-MenuBar::MENUBAR_HEIGHT-TITLE_SIZE));
        v->setBodyPos(&bp);
        win->addElement(v);

        Label* separator = new Label("", sf::Vector2i(CONSTANT::WINDOW_WIDTH*0.6-2, MenuBar::MENUBAR_HEIGHT+TITLE_SIZE), sf::Vector2i(4, CONSTANT::WINDOW_HEIGHT-MenuBar::MENUBAR_HEIGHT-TITLE_SIZE), 16, Label::CENTER, CONSTANT::COLOR_BACK);
        win->addElement(separator);

        Button* trackBtn = new Button("Start tracking", sf::Vector2i(CONSTANT::WINDOW_WIDTH*0.725, CONSTANT::WINDOW_HEIGHT-90), sf::Vector2i(CONSTANT::WINDOW_WIDTH*0.15, 70), 20);
            trackBtn->setText( camMan.isTracking()? "Stop tracking": "Start tracking" );
            trackBtn->setTextColor( camMan.isTracking()? CONSTANT::COLOR_RED_LIGHT: CONSTANT::COLOR_PRIMARY );
        camMan.attachButton(trackBtn);
        trackBtn->setCallback(&CameraManager::toogleCameraTracking, &camMan);
        win->addElement(trackBtn);
    }

    void GenerateOptionsScene()
    {
        win->clearElements();
        win->addElement(createMenuBar(OPTIONS_SCENE));

        win->addElement(new Title(sf::Vector2i(CONSTANT::WINDOW_WIDTH*0.4, MenuBar::MENUBAR_HEIGHT), sf::Vector2i(CONSTANT::WINDOW_WIDTH*0.2, 40), " Options ", 20, CONSTANT::COLOR_CLEAR));
        List* list = new List(sf::Vector2i(0, MenuBar::MENUBAR_HEIGHT+40), sf::Vector2i(CONSTANT::WINDOW_WIDTH, CONSTANT::WINDOW_HEIGHT-MenuBar::MENUBAR_HEIGHT+40), CONSTANT::COLOR_BACK);
        win->addElement(list);

        Label* l1 = new Label("Show elements hitboxes", sf::Vector2i(10, 10), sf::Vector2i(200, 50), 16, Label::LEFT, CONSTANT::COLOR_FORE, CONSTANT::COLOR_BACK);
        Checkbox* b1 = new Checkbox(sf::Vector2i(250, -35), sf::Vector2i(20, 20), CONSTANT::COLOR_PRIMARY, CONSTANT::COLOR_BACK);
        b1->setChecked(win->doesShowhitboxes());
        b1->setCallback(&Window::setShowHitBoxes, win);
        list->addComponent(l1);
        list->addComponent(b1);
        
        l1 = new Label("Tracking FPS", sf::Vector2i(10, 10), sf::Vector2i(200, 50), 16, Label::LEFT, CONSTANT::COLOR_FORE, CONSTANT::COLOR_BACK);
        Input* i1 = new Input(std::to_string(CONSTANT::TRACKING_FPS), sf::Vector2i(250, -40), sf::Vector2i(50, 30), 16, Input::CENTER, CONSTANT::COLOR_FORE, "20");
        i1->setCallback(_set_tracking_fps);
        list->addComponent(l1);
        list->addComponent(i1);
        
        l1 = new Label("Cameras FPS", sf::Vector2i(10, 10), sf::Vector2i(200, 50), 16, Label::LEFT, CONSTANT::COLOR_FORE, CONSTANT::COLOR_BACK);
        i1 = new Input(std::to_string(CONSTANT::CAMERA_FPS), sf::Vector2i(250, -40), sf::Vector2i(50, 30), 16, Input::CENTER, CONSTANT::COLOR_FORE, "30");
        i1->setCallback(_set_camera_fps);
        list->addComponent(l1);
        list->addComponent(i1);
        
        Label* l2 = new Label("Smooth tracked position", sf::Vector2i(10, 10), sf::Vector2i(200, 50), 16, Label::LEFT, CONSTANT::COLOR_FORE, CONSTANT::COLOR_BACK);
        Checkbox* b2 = new Checkbox(sf::Vector2i(250, -35), sf::Vector2i(20, 20), CONSTANT::COLOR_PRIMARY, CONSTANT::COLOR_BACK);
        b2->setChecked(bp.getSmoothPosition());
        b2->setCallback(&BodyPos::setSmoothPosition, &bp);
        list->addComponent(l2);
        list->addComponent(b2);
    }

    void GenerateExtensionsScene()
    {
        win->clearElements();
        win->addElement(createMenuBar(EXTENSIONS_SCENE));
        win->addElement(new Title(sf::Vector2i(CONSTANT::WINDOW_WIDTH*0.4, MenuBar::MENUBAR_HEIGHT), sf::Vector2i(CONSTANT::WINDOW_WIDTH*0.2, 40), " Extensions ", 20, CONSTANT::COLOR_CLEAR));
        
        Container* cont = new Container(sf::Vector2i(0, MenuBar::MENUBAR_HEIGHT+40), sf::Vector2i(CONSTANT::WINDOW_WIDTH, CONSTANT::WINDOW_HEIGHT-MenuBar::MENUBAR_HEIGHT-40), CONSTANT::COLOR_BACK);
        cont->addElement(new Label("PORT: "+std::to_string(con.getPort()), sf::Vector2i(CONSTANT::WINDOW_WIDTH-140, 10), sf::Vector2i(120, 40), 20, Label::CENTER, CONSTANT::COLOR_FORE, CONSTANT::COLOR_CLEAR));
        cont->addElement(new Label("IP: "+con.getIP(), sf::Vector2i(CONSTANT::WINDOW_WIDTH-340, 10), sf::Vector2i(180, 40), 20, Label::CENTER, CONSTANT::COLOR_FORE, CONSTANT::COLOR_CLEAR));

        win->addElement(cont);
        cont->addElement(new Label("Active extensions:", sf::Vector2i(10, 20), sf::Vector2i(400, 40), 18, Label::LEFT, CONSTANT::COLOR_FORE, CONSTANT::COLOR_BACK));
        extensionList = new List(sf::Vector2i(10, 60), sf::Vector2i(CONSTANT::WINDOW_WIDTH*0.7, CONSTANT::WINDOW_HEIGHT/2), CONSTANT::COLOR_CLEAR);
        cont->addComponent(extensionList);
    }

    void GenerateAboutScene()
    {
        win->clearElements();
        win->addElement(createMenuBar(ABOUT_SCENE));

        win->addElement(new Title(
            sf::Vector2i(CONSTANT::WINDOW_WIDTH*0.3, MenuBar::MENUBAR_HEIGHT),
            sf::Vector2i(CONSTANT::WINDOW_WIDTH*0.4, 40), " Credits ", 20,
            CONSTANT::COLOR_CLEAR
        ));
        win->addElement(new Container(
            sf::Vector2i(20, 60+MenuBar::MENUBAR_HEIGHT),
            sf::Vector2i(CONSTANT::WINDOW_WIDTH-40, 200),
            CONSTANT::COLOR_BACK
        ));
        win->addElement(new Label(
            "Author: FurWaz", sf::Vector2i(30, 70+MenuBar::MENUBAR_HEIGHT),
            sf::Vector2i(CONSTANT::WINDOW_WIDTH-260, 30), 18, Label::LEFT,
            CONSTANT::COLOR_FORE, CONSTANT::COLOR_BACK
        ));
        win->addElement(new Label(
            "https://furwaz.github.io/", sf::Vector2i(30, 100+MenuBar::MENUBAR_HEIGHT),
            sf::Vector2i(190, 30), 16, Label::LEFT,
            CONSTANT::COLOR_BLUE_LIGHT, CONSTANT::COLOR_BACK
        ));
        win->addElement(new Image(
            "./resources/pics/furwaz.png",
            sf::Vector2i(CONSTANT::WINDOW_WIDTH-220, 60+MenuBar::MENUBAR_HEIGHT), sf::Vector2i(200, 200),
            CONSTANT::COLOR_BACK
        ));
    }

    GraphicElement* createMenuBar(int pos)
    {
        MenuBar* menuBar = new MenuBar();
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
        menuBar->setSelectedButton(pos);
        CURRENT_SCENE = pos;
        return menuBar;
    }

    // Buttons callbacks
    void _add_camera()
    {
        cameras.push_back(new Camera());
        _populate_camlist(true, true);
    }
    void _rem_camera()
    {
        if (cameras.size() == 0) return;
        cameras.erase(cameras.end()-1);
        _populate_camlist(true, false);
    }
    void _set_tracking_fps(std::string fps)
    {
        try
        {
            CONSTANT::TRACKING_FPS = std::stoi(fps);
            if (CONSTANT::TRACKING_FPS == 0)
                CONSTANT::TRACKING_FPS = 20;
        } catch (std::exception &e)
        {
            CONSTANT::TRACKING_FPS = 20;
        }
        CONSTANT::updateFPSdelta();
    }
    void _set_camera_fps(std::string fps)
    {
        try
        {
            CONSTANT::CAMERA_FPS = std::stoi(fps);
            if (CONSTANT::CAMERA_FPS == 0)
                CONSTANT::CAMERA_FPS = 30;
        } catch (std::exception &e)
        {
            CONSTANT::CAMERA_FPS = 30;
        }
        CONSTANT::updateFPSdelta();
    }
    void _populate_camlist(bool modify, bool added)
    {
        bp.setCameras(cameras);
        camMan.setCameras(cameras);
        if (!modify)
        {
            camList->clearComponents();
            for (Camera* cam: cameras)
                camList->addComponent(new CameraView(cam, sf::Vector2i(10, 10), sf::Vector2i(camList->getSize().x-20, 400)));
        } else
        {
            if (added)
                camList->addComponent(new CameraView(cameras[cameras.size()-1], sf::Vector2i(10, 10), sf::Vector2i(camList->getSize().x-20, 400)));
            else
                camList->removeComponent(camList->getElements().size()-1);
        }
    }
    void _check_for_new_ext()
    {
        shouldUpdate = true;
    }
}