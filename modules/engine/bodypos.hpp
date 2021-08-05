#pragma once
#include "../constants.hpp"
#include "./camera.hpp"
#include <opencv2/core.hpp>
#include <vector>
#include <chrono>
#include "./softwareConnection.hpp"

SoftwareConnection con;

namespace owo
{
    /**
     * @brief BodyPos class, calculates all the camera body rays based on the camera's tracker's detected points.
     * Also calculates the final 3D body position based on the camera's rays
     */
    class BodyPos
    {
    private:
        Camera* cam1;
        Camera* cam2;

        cv::Vec3d cam1_rays[CONSTANT::NB_JOINTS];
        cv::Vec3d cam2_rays[CONSTANT::NB_JOINTS];
        cv::Vec3d body[CONSTANT::NB_JOINTS];

        float tracking_dt;

        /**
         * @brief Returns a new rotated point based on the point's position and a rotation matrix
         * 
         * @param vec the point's original position
         * @param rot the rotation matrix
         * @return A new rotated 3D point
         */
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

        /**
         * @brief Calculates the intersection between two 3D vectors (from top view) into [r]
         * 
         * @param o1 first vector origin
         * @param p1 first vector direction
         * @param o2 second vector origin
         * @param p2 second vection direction
         * @param r the calculated intersection point
         * @return If the intersection has been found or not
         */
        bool intersection(cv::Vec3d o1, cv::Vec3d p1, cv::Vec3d o2, cv::Vec3d p2,
                      cv::Vec3d &r)
        {
            cv::Vec3d x = o2 - o1;
            cv::Vec3d d1 = p1 - o1;
            cv::Vec3d d2 = p2 - o2;

            double cross = d1[0]*d2[1] - d1[1]*d2[0];
            if (abs(cross) < 1e-8)
                return false;

            double t1 = (x[0] * d2[1] - x[1] * d2[0])/cross;
            r = o1 + d1 * t1;
            return true;
        }

        /**
         * @brief Calculated the camera's body joint rays based on the camera's informations and camera's tracker
         * 
         * @param cam The targeted camera for rays calculation
         * @param rays The calculaterd rays array
         */
        void calculateCamRays(Camera* cam, cv::Vec3d* rays)
        {
            std::vector<cv::Point3f> points = cam->getTracker()->getPoints();
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

        /**
         * @brief Calculates the body's final 3D position based on the two camera's rays arrays
         */
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
            con.sendNewBodyPosition(this->getBody());
        }

    public:
        /**
         * @brief Default constructor
         */
        BodyPos()
        {
            for(int i = 0; i < 33; i++)
            {
                this->body[i]      = cv::Vec3d();
                this->cam1_rays[i] = cv::Vec3d();
                this->cam2_rays[i] = cv::Vec3d();
            }
            this->tracking_dt = 0;
            con.startConnection();
        }

        /**
         * @brief Set the Camera1 object
         * 
         * @param cam The targeted camera
         */
        void setCamera1(Camera* cam)
        {
            this->cam1 = cam;
            this->cam1->getTracker()->setNewDataCallback(&BodyPos::refreshBody, this);
        }
        
        /**
         * @brief Set the Camera2 object
         * 
         * @param cam The targeted camera
         */
        void setCamera2(Camera* cam)
        {
            this->cam2 = cam;
            this->cam2->getTracker()->setNewDataCallback(&BodyPos::refreshBody, this);
        }

        /**
         * @brief Asks the cameras for new tracking data to update the body
         */
        void update(float dt)
        {
            this->tracking_dt += dt * 1000;
            if ( this->tracking_dt < CONSTANT::TRACKING_FPS_DELTA )
                return;

            this->tracking_dt = 0;
            this->cam1->getBodyPosition();
            this->cam2->getBodyPosition();
        }

        /**
         * @brief [INTERNAL] Updates camera rays and calculates the new body position
         */
        void refreshBody()
        {
            if (!this->cam1->getTracker()->isNewTrackingDataAvailable() || !this->cam2->getTracker()->isNewTrackingDataAvailable())
                return;
            this->cam1->getTracker()->setNewTrackingDataAvailable(false);
            this->cam2->getTracker()->setNewTrackingDataAvailable(false);
            this->calculateCamRays(this->cam1, this->cam1_rays);
            this->calculateCamRays(this->cam2, this->cam2_rays);
            this->calculateBodyPosition();
        }

        /**
         * @brief Returns the body's 3D joints array
         * @return An array of 3D points
         */
        cv::Vec3d* getBody()
        {
            return this->body;
        }

        /**
         * @brief Returns the first camera's joints rays
         * @return An array of 3D vectors directions
         */
        cv::Vec3d* getCamRays1()
        {
            return this->cam1_rays;
        }

        /**
         * @brief Returns the second camera's joints rays
         * @return An array of 3D vectors directions
         */
        cv::Vec3d* getCamRays2()
        {
            return this->cam2_rays;
        }

        ~BodyPos()
        {

        }
    };
}