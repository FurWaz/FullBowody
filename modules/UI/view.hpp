#pragma once
#include "../constants.hpp"
#include "graphicElement.hpp"
#include "../engine/camera.hpp"
#include "../engine/bodypos.hpp"
#include "opencv2/core.hpp"

namespace owo
{
    class View : public virtual GraphicElement
    {
    private:
        float camDist;
        sf::Vector2f rotation;
        
        cv::Vec3d camPos;
        cv::Vec3d camRot;
        float camFOV;

        sf::Vector2i lastMousePos;

        Camera* camObj;
        Camera* camObj2;

        BodyPos bp;

        void init()
        {
            this->renderTexture.create(this->dimensions.width, this->dimensions.height);
            this->sprite.setPosition(this->dimensions.left, this->dimensions.top);
            this->camDist = 2;
            camFOV = 200;
            this->rotation = sf::Vector2f(0.75, 0.75);
            apply_rotation();
        }

        void apply_rotation()
        {
            this->camRot[0] = this->rotation.x;
            this->camRot[1] = this->rotation.y;
            this->camPos[0] = std::cos(this->rotation.x) * -camDist * std::sin(this->rotation.y);
            this->camPos[1] = std::cos(this->rotation.x) *  camDist * std::cos(this->rotation.y);
            this->camPos[2] = std::sin(this->rotation.x) *  camDist;
        }

        sf::Vector2f vec3_vec2(cv::Vec3d coord)
        {
            if (coord[2] - camPos[2] == 0)
                return sf::Vector2f();
            // Inverted Z and Y axis for visual convinience
            float distX = coord[0]-camPos[0];
            float distY = coord[1]-camPos[1];
            float distZ = coord[2]-camPos[2];
            float sinRotY = std::sin(camRot[1]);
            float sinRotX = std::sin(camRot[0]);
            float cosRotY = std::cos(camRot[1]);
            float cosRotX = std::cos(camRot[0]);
            float Zresult = (distY * cosRotY) - (distX * sinRotY);
            float Zresult2 = (distZ * sinRotX) + (Zresult * cosRotX);
            float Xresult = (distY * sinRotY) + (distX * cosRotY);
            float Yresult = (distZ * cosRotX) - (Zresult * sinRotX);
            if (Zresult2 > 0)
                return sf::Vector2f();
            return sf::Vector2f(camFOV * (Xresult / Zresult2) + this->dimensions.width/2, camFOV * (Yresult / Zresult2) + this->dimensions.height/2);
        }

        void clear_tex()
        {
            this->renderTexture.clear(CONSTANT::COLOR_CLEAR);
        }

        void draw_line(sf::Vector2f p1, sf::Vector2f p2, sf::Color color = CONSTANT::COLOR_FORE)
        {
            sf::Vertex line[] = {sf::Vertex(p1, color), sf::Vertex(p2, color)};
            this->renderTexture.draw(line, 2, sf::Lines);
        }

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

        void draw_origin(cv::Vec3d pos, cv::Mat rot, float size = 1.f)
        {
            draw_line(vec3_vec2(rotate(cv::Vec3d(size, 0, 0), rot) + pos), vec3_vec2(rotate(cv::Vec3d(-size, 0, 0), rot) + pos), CONSTANT::COLOR_RED_LIGHT);
            draw_line(vec3_vec2(rotate(cv::Vec3d(0, size, 0), rot) + pos), vec3_vec2(rotate(cv::Vec3d(0, -size, 0), rot) + pos), CONSTANT::COLOR_GREEN_LIGHT);
            draw_line(vec3_vec2(rotate(cv::Vec3d(0, 0, size), rot) + pos), vec3_vec2(rotate(cv::Vec3d(0, 0, -size), rot) + pos), CONSTANT::COLOR_BLUE_LIGHT);
        }

        void draw_camera(cv::Vec3d pos, cv::Mat rot, float size = 1.f)
        {
            sf::Vector2f p1 = vec3_vec2(rotate(cv::Vec3d( size*0.2, size*0.4, size*0.5), rot) + pos);
            sf::Vector2f p2 = vec3_vec2(rotate(cv::Vec3d( size*0.2,-size*0.4, size*0.5), rot) + pos);
            sf::Vector2f p3 = vec3_vec2(rotate(cv::Vec3d(-size*0.2,-size*0.4, size*0.5), rot) + pos);
            sf::Vector2f p4 = vec3_vec2(rotate(cv::Vec3d(-size*0.2, size*0.4, size*0.5), rot) + pos);
            draw_line(vec3_vec2(rotate(cv::Vec3d(0, 0, size), rot) + pos), vec3_vec2(pos), CONSTANT::COLOR_BLUE_LIGHT);
            draw_line(p1, vec3_vec2(pos), CONSTANT::COLOR_ORANGE_LIGHT);
            draw_line(p2, vec3_vec2(pos), CONSTANT::COLOR_ORANGE_LIGHT);
            draw_line(p3, vec3_vec2(pos), CONSTANT::COLOR_ORANGE_LIGHT);
            draw_line(p4, vec3_vec2(pos), CONSTANT::COLOR_ORANGE_LIGHT);
            draw_line(p1, p4, CONSTANT::COLOR_ORANGE_LIGHT);
            draw_line(p2, p1, CONSTANT::COLOR_ORANGE_LIGHT);
            draw_line(p3, p2, CONSTANT::COLOR_ORANGE_LIGHT);
            draw_line(p4, p3, CONSTANT::COLOR_ORANGE_LIGHT);
        }

        void draw_body()
        {
            cv::Vec3d* points = this->bp.getBody();
            for(int i = 0; i < 35; i++)
                draw_line(
                    vec3_vec2(points[CONSTANT::POSE_CONNECTIONS[i][0]]),
                    vec3_vec2(points[CONSTANT::POSE_CONNECTIONS[i][1]]),
                    CONSTANT::COLOR_PURPLE_LIGHT
                );
        }

        void apply_tex()
        {
            this->renderTexture.display();
            this->sprite.setTexture(this->renderTexture.getTexture());
        }

    public:
        View()
        {
            this->setDimensions(0, 0, 200, 200);
            init();
        }

        View(sf::Vector2i pos, sf::Vector2i size)
        {
            this->setDimensions(pos.x, pos.y, size.x, size.y);
            init();
        }

        void onClick(int btn, bool clicked)
        {
            this->clicked = clicked;
        }

        void onFocus(bool focused)
        {
            this->focused = focused;
        }

        void onHover(bool hovered)
        {
            this->hovered = hovered;
        }
        
        void onKey(int key, char c, bool pressed)
        {
            
        }

        void onScroll(int delta)
        {
            camDist -= delta * 0.001;
            apply_rotation();
        }

        void update(float dt, sf::Vector2i mousePos)
        {
            if (clicked)
            {
                sf::Vector2i delta = mousePos - lastMousePos;
                this->rotation.x += delta.y * 0.01;
                this->rotation.y -= delta.x * 0.01;
            }
            apply_rotation();
            this->bp.update();
            lastMousePos = mousePos;
        }

        sf::Sprite getSprite(float dt)
        {
            clear_tex();
            draw_origin(cv::Vec3d(0, 0, 0), cv::Mat::eye(cv::Size(3, 3), CV_64F), 0.5);
            cv::Vec3d pos = camObj->getPosition();
            cv::Mat rot = camObj->getRotation();
            draw_camera(pos, rot, 0.4);
            // cv::Vec3d* rays = this->bp.getCamRays1();
            // for(int i = 0; i < 35; i++)
            //     draw_line(
            //         vec3_vec2(rotate(rays[CONSTANT::POSE_CONNECTIONS[i][0]], rot) + pos),
            //         vec3_vec2(rotate(rays[CONSTANT::POSE_CONNECTIONS[i][1]], rot) + pos),
            //         CONSTANT::COLOR_PURPLE_LIGHT
            //     );
            pos = camObj2->getPosition();
            rot = camObj2->getRotation();
            draw_camera(pos, rot, 0.4);
            draw_body();

            apply_tex();
            return this->sprite;
        }

        void setCameraObj(Camera* cam)
        {
            this->camObj = cam;
            this->bp.setCamera1(cam);
        }

        void setCameraObj2(Camera* cam)
        {
            this->camObj2 = cam;
            this->bp.setCamera2(cam);
        }

        ~View()
        {

        }

    };
}