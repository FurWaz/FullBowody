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

        cv::Vec3d cam1_rays[CONSTANT::NB_JOINTS];
        cv::Vec3d cam2_rays[CONSTANT::NB_JOINTS];

        cv::Vec3d body[CONSTANT::NB_JOINTS];

        cv::Vec3d rotate(cv::Vec3d vec, cv::Mat rot)
        {
            cv::Mat res = cv::Mat(cv::Size(1, 3), CV_64F);
            res.at<double>(0, 0) = vec[0];
            res.at<double>(1, 0) = vec[1];
            res.at<double>(2, 0) = vec[2];
            try {
                res = rot * res;
            } catch (std::exception &e) {std::cout << e.what() << std::endl;}
            return res;
        }

        bool intersection(cv::Vec3d o1, cv::Vec3d p1, cv::Vec3d o2, cv::Vec3d p2,
                      cv::Vec3d &r)
        {
            cv::Vec3d x = o2 - o1;
            cv::Vec3d d1 = p1 - o1;
            cv::Vec3d d2 = p2 - o2;

            double cross = d1[0]*d2[1] - d1[1]*d2[0];
            if (abs(cross) < 1e-8) /*EPS*/
                return false;

            double t1 = (x[0] * d2[1] - x[1] * d2[0])/cross;
            r = o1 + d1 * t1;
            return true;
        }

        void calculateCamRays(Camera* cam, cv::Vec3d* rays)
        {
            std::vector<cv::Point3f> points = cam->getTracker()->getPoints(); // x, y, visibility (from 0 to 1)
            cv::Mat rot = cam->getRotation();
            cv::Point2d fov = cam->getFOV();
            for(int i = 0; i < points.size(); i++)
            {
                const cv::Point3f p = points[i];
                double rotX = ((p.y - 0.5) * 2) * fov.y;
                double rotY = ((p.x - 0.5) * 2) * fov.x;
                rays[i] = this->rotate(cv::Vec3d(-std::sin(rotY), std::sin(rotX), std::cos(rotX) * std::cos(rotY)), rot);
            }
        }

        void calculateBodyPosition()
        {
            cv::Vec3d c1 = this->cam1->getPosition();
            cv::Vec3d c2 = this->cam2->getPosition();
            for(int i = 0; i < CONSTANT::NB_JOINTS; i++)
            {
                cv::Vec3d v1 = cam1_rays[i];
                cv::Vec3d v2 = cam2_rays[i];
                this->intersection(c1, v1, c2, v2, this->body[i]);
            }
        }

    public:
        BodyPos()
        {
            for(int i = 0; i < 33; i++)
            {
                this->body[i]      = cv::Vec3d();
                this->cam1_rays[i] = cv::Vec3d();
                this->cam2_rays[i] = cv::Vec3d();
            }
        }

        void setCamera1(Camera* cam)
        {
            this->cam1 = cam;
        }

        void setCamera2(Camera* cam)
        {
            this->cam2 = cam;
        }

        void update()
        {
            this->calculateCamRays(this->cam1, this->cam1_rays);
            this->calculateCamRays(this->cam2, this->cam2_rays);
            this->calculateBodyPosition();
        }

        cv::Vec3d* getBody()
        {
            return this->body;
        }

        cv::Vec3d* getCamRays1()
        {
            return this->cam1_rays;
        }

        cv::Vec3d* getCamRays2()
        {
            return this->cam2_rays;
        }

        ~BodyPos()
        {

        }

    };
}