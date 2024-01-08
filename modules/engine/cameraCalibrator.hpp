#pragma once
#include <opencv2/core.hpp>
#include "../constants.hpp"
#include "./camera.hpp"
#include "../UI/window.hpp"
#include "../UI/calibrationPopup.hpp"

using namespace owo;

namespace CameraCalibrator
{
    Window* window;
    Camera* camera;
    CalibrationPopup* calibPopup;

    void setWindow(Window* win)
    {
        window = win;
    }

    void _cancel_calibration()
    {
        window->removeElement(calibPopup);
        calibPopup->cancel();
    }

    void calibrateCamera(Camera* cam)
    {
        camera = cam;
        calibPopup = new CalibrationPopup(cam);
        calibPopup->getPopup()->getCloseButton()->setCallback(CameraCalibrator::_cancel_calibration);
        window->addElement(calibPopup);
    }
};