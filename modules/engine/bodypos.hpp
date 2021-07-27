#pragma once
#include "../constants.hpp"
#include "./camera.hpp"
#include <opencv2/core.hpp>
#include <vector>

namespace owo
{
    class BodyPos
    {
    private:

        Camera* cam1;
        Camera* cam2;

        cv::Point3f cam1_rays[CONST::NB_JOINTS];
        cv::Point3f cam2_rays[CONST::NB_JOINTS];

        cv::Point3f body[CONST::NB_JOINTS];

        void calculateCamRays(Camera* cam, cv::Point3f* rays)
        {

        }

    public:
        BodyPos()
        {
            for(int i = 0; i < 33; i++)
            {
                this->body[i] = cv::Point3f(0, 0, 0);
                this->cam1_rays[i] = cv::Point3f(0, 0, 0);
                this->cam2_rays[i] = cv::Point3f(0, 0, 0);
            }
        }

        void addCamera1(Camera* cam)
        {
            this->cam1 = cam;
        }

        void addCamera2(Camera* cam)
        {
            this->cam2 = cam;
        }

        void update(float dt, sf::Vector2i mousePos)
        {
            this->
        }

        ~BodyPos()
        {

        }

    };
}