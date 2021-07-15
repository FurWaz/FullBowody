#pragma once
#include "./window.hpp"
#include "./button.hpp"
#include "./image.hpp"
#include "./fpsCounter.hpp"
#include "./label.hpp"
#include "./input.hpp"
#include "./cameraView.hpp"

#include "../engine/camera.hpp"
#include "../engine/cameraManager.hpp"

using namespace owo;

namespace SceneGenerator
{
    void GenerateDefaultScene(Window &win)
    {
        int cameraWidth = 250;
        CameraView* camView = new CameraView(new Camera(), sf::Vector2i(10, 10), sf::Vector2i(cameraWidth, win.getHeight()-20));
        CameraView* camView2 = new CameraView(new Camera(), sf::Vector2i(win.getWidth()-cameraWidth-10, 10), sf::Vector2i(cameraWidth, win.getHeight()-20));

        win.addElement(camView);
        for(GraphicElement* el: camView->getElements())
            win.addElement(el);
        
        win.addElement(camView2);
        for(GraphicElement* el: camView2->getElements())
            win.addElement(el);
        
        camView2->getCamera()->openSource("http://192.168.1.39:8080/video");
        Button* btn = new Button(
            "Start tracking",
            sf::Vector2i(win.getWidth()/2-100, 10), sf::Vector2i(200, 50),
            CONST::COLOR_BACKGROUND, CONST::COLOR_PRIMARY
        );
        win.addElement(btn);

        CameraManager* camManager = new CameraManager();
        btn->setCallback(&CameraManager::toogleCameraTracking, camManager);
    }
}