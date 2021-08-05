#pragma once
#include "./window.hpp"
#include "./button.hpp"
#include "./image.hpp"
#include "./fpsCounter.hpp"
#include "./label.hpp"
#include "./input.hpp"
#include "./cameraView.hpp"
#include "./view.hpp"
#include "./list.hpp"

#include "../engine/camera.hpp"
#include "../engine/cameraManager.hpp"

using namespace owo;

namespace SceneGenerator
{
    void GenerateDefaultScene(Window &win)
    {
        win.clearElements();
        int cameraWidth = CONSTANT::WINDOW_WIDTH*0.25;

        View* view = new View(
                sf::Vector2i(0, 0),
                sf::Vector2i(CONSTANT::WINDOW_WIDTH, CONSTANT::WINDOW_HEIGHT)
        );
        win.addElement(view);

        CameraView* camView = new CameraView(new Camera(), sf::Vector2i(10, 10), sf::Vector2i(cameraWidth, CONSTANT::WINDOW_HEIGHT-20));
        CameraView* camView2 = new CameraView(new Camera(), sf::Vector2i(CONSTANT::WINDOW_WIDTH-cameraWidth-10, 10), sf::Vector2i(cameraWidth, CONSTANT::WINDOW_HEIGHT-20));

        view->setCamera1(camView->getCamera());
        view->setCamera2(camView2->getCamera());

        camView->getCamera()->loadCalibration("./note4_x.txt");
        camView2->getCamera()->loadCalibration("./note4_x.txt");

        win.addElement(camView);
        win.addElement(camView2);
        
        Button* btn = new Button(
            "Start tracking",
            sf::Vector2i(CONSTANT::WINDOW_WIDTH/2-100, 10),
            sf::Vector2i(200, 50), 20,
            CONSTANT::COLOR_BACK, CONSTANT::COLOR_PRIMARY
        );
        win.addElement(btn);

        CameraManager* camManager = new CameraManager();
        camManager->addCamera(camView->getCamera());
        camManager->addCamera(camView2->getCamera());
        camManager->attachButton(btn);
        btn->setCallback(&CameraManager::toogleCameraTracking, camManager);

        win.addElement(new FPSCounter(
            sf::Vector2i(CONSTANT::WINDOW_WIDTH/2 - 50, CONSTANT::WINDOW_HEIGHT-50),
            sf::Vector2i(100, 40)
        ));
    }

    void GenerateTestScene(Window &win)
    {
        win.clearElements();
        List* l = new List(sf::Vector2i(20, 20), sf::Vector2i(150, CONSTANT::WINDOW_HEIGHT-40), CONSTANT::COLOR_BACK);
        Button* b1 = new Button("First", sf::Vector2i(10, 10), sf::Vector2i(100, 50), 20, CONSTANT::COLOR_RED_LIGHT, CONSTANT::COLOR_CLEAR);
        Button* b2 = new Button("Second", sf::Vector2i(10, 10), sf::Vector2i(100, 50), 20, CONSTANT::COLOR_RED_LIGHT, CONSTANT::COLOR_CLEAR);
        Button* b3 = new Button("Third", sf::Vector2i(10, 10), sf::Vector2i(100, 50), 20, CONSTANT::COLOR_RED_LIGHT, CONSTANT::COLOR_CLEAR);
        l->addComponent(b1);
        l->addComponent(b2);
        l->addComponent(b3);
        l->propagateParentAbsPos();
        win.addElement(l);
    }
}