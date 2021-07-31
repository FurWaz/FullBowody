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
        win.clearElements();
        int cameraWidth = CONSTANT::WINDOW_WIDTH*0.25;

        View* view = new View(
                sf::Vector2i(cameraWidth, 0),
                sf::Vector2i(CONSTANT::WINDOW_WIDTH-cameraWidth*2, CONSTANT::WINDOW_HEIGHT)
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

        //win.addElement(new FPSCounter());
    }
}