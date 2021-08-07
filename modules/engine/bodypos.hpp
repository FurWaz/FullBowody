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
        std::vector<Camera*> cameras;
        std::vector<cv::Vec3d*> rays;
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
            for (int i = 0; i < CONSTANT::NB_JOINTS; i++)
            {
                int cam1 = 0;
                int cam2 = 1;

                // determine the two cameras with the best point visibility
                float visibility1 = this->cameras.at(cam1)->getTracker()->getPoints().at(i).z;
                float visibility2 = this->cameras.at(cam2)->getTracker()->getPoints().at(i).z;
                for (int j = 2; j < this->cameras.size(); j++)
                {
                    float v = this->cameras.at(j)->getTracker()->getPoints().at(i).z;
                    if (v > visibility1 && v > visibility2)
                    {
                        if (visibility1 > visibility2)
                            cam2 = j;
                        else cam1 = j;
                    }
                    else if (v > visibility1) cam1 = j;
                    else if (v > visibility2) cam2 = j;
                }

                // calculate the point position
                this->intersection(
                    this->cameras.at(cam1)->getPosition(), *this->rays.at(cam1),
                    this->cameras.at(cam2)->getPosition(), *this->rays.at(cam2),
                    this->body[i]
                );
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
                this->body[i] = cv::Vec3d();
            this->tracking_dt = 0;
            con.startConnection();
        }

        /**
         * @brief Adds a new Camera object
         * @param cam The targeted camera
         */
        void AddCamera(Camera* cam)
        {
            this->cameras.push_back(cam);
            cv::Vec3d newRays[CONSTANT::NB_JOINTS];
            this->rays.push_back(newRays);
            cam->getTracker()->setNewDataCallback(&BodyPos::refreshBody, this);
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
            for (Camera* cam: this->cameras)
                cam->getBodyPosition();
        }

        /**
         * @brief [INTERNAL] Updates camera rays and calculates the new body position
         */
        void refreshBody()
        {
            for (Camera* cam: this->cameras)
                if (!cam->getTracker()->isNewTrackingDataAvailable())
                    return;
                    
            for(int i = 0; i < this->cameras.size(); i++)
            {
                Camera* c = this->cameras.at(i);
                cv::Vec3d* r = this->rays.at(i);
                c->getTracker()->setNewTrackingDataAvailable(false);
                this->calculateCamRays(c, r);
            }
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
         * @brief Returns the selected camera's joints rays
         * @return An array of 3D vectors directions
         */
        cv::Vec3d* getCamRays(int camIndex)
        {
            return this->rays.at(camIndex);
        }

        std::vector<Camera*> getCameras()
        {
            return this->cameras;
        }

        ~BodyPos()
        {

        }
    };
}