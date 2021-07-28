#pragma once
#include "./window.hpp"
#include "./button.hpp"
#include "./image.hpp"
#include "./fpsCounter.hpp"
#include "./label.hpp"
#include "./input.hpp"
#include "./cameraView.hpp"
#include "./view.hpp"

#include "../engine/camera.hpp"
#include "../engine/cameraManager.hpp"

using namespace owo;

namespace SceneGenerator
{
    void GenerateDefaultScene(Window &win)
    {
        int cameraWidth = 300;

        View* view = new View(
                sf::Vector2i(cameraWidth, 0),
                sf::Vector2i(win.getWidth()-cameraWidth*2, win.getHeight())
        );
        win.addElement(view);

        CameraView* camView = new CameraView(new Camera(), sf::Vector2i(10, 10), sf::Vector2i(cameraWidth, win.getHeight()-20));
        CameraView* camView2 = new CameraView(new Camera(), sf::Vector2i(win.getWidth()-cameraWidth-10, 10), sf::Vector2i(cameraWidth, win.getHeight()-20));

        view->setCameraObj(camView->getCamera());
        view->setCameraObj2(camView2->getCamera());

        camView->getCamera()->loadCalibration("./out.txt");
        camView2->getCamera()->loadCalibration("./out.txt");

        win.addElement(camView);
        for(GraphicElement* el: camView->getElements())
            win.addElement(el);
        
        win.addElement(camView2);
        for(GraphicElement* el: camView2->getElements())
            win.addElement(el);
        
        Button* btn = new Button(
            "Start tracking",
            sf::Vector2i(win.getWidth()/2-100, 10), sf::Vector2i(200, 50),
            CONSTANT::COLOR_BACK, CONSTANT::COLOR_PRIMARY
        );
        win.addElement(btn);

        CameraManager* camManager = new CameraManager();
        camManager->addCamera(camView->getCamera());
        camManager->addCamera(camView2->getCamera());
        btn->setCallback(&CameraManager::toogleCameraTracking, camManager);

        //win.addElement(new FPSCounter());
    }
}