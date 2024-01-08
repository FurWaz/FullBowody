#pragma once
#include <opencv2/core.hpp>
#include "../../constants.hpp"
#include "./graphicElement.hpp"
#include "../../engine/camera.hpp"
#include "../../engine/bodypos.hpp"

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

        BodyPos* bp;

        void init()
        {
            this->renderTexture.create(this->dimensions.width, this->dimensions.height);
            this->sprite.setPosition(this->dimensions.left, this->dimensions.top);
            this->camDist = 1;
            camFOV = 200;
            this->rotation = sf::Vector2f(0.6, 0.8);
            apply_rotation();
            this->setReceiveEvents(true);
        }

        void apply_rotation()
        {
            this->camRot[0] = this->rotation.x;
            this->camRot[1] = this->rotation.y;
            this->camPos[0] = std::cos(this->rotation.x) * -camDist * std::sin(this->rotation.y);
            this->camPos[1] = std::cos(this->rotation.x) *  camDist * std::cos(this->rotation.y);
            this->camPos[2] = std::sin(this->rotation.x) *  camDist + 1;
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
                return sf::Vector2f(this->dimensions.width/2, this->dimensions.height/2);
            return sf::Vector2f(camFOV * (Xresult / Zresult2) + this->dimensions.width/2, camFOV * (Yresult / Zresult2) + this->dimensions.height/2);
        }

        void clear_tex()
        {
            this->renderTexture.clear(this->clearColor);
        }

        void draw_line(sf::Vector2f p1, sf::Vector2f p2, sf::Color color = CONSTANT::COLOR_FORE, int thinkness = 1)
        {
            float delta = (p2.y - p1.y) / (p2.x - p1.x);
            float rotation = std::atan(delta);
            float length = std::sqrt( std::pow(p2.x - p1.x, 2) + std::pow(p2.y - p1.y, 2) );
            sf::RectangleShape rect(sf::Vector2f(length, thinkness));
            rect.setPosition((p1.x+p2.x)*0.5f, (p1.y+p2.y)*0.5f);
            rect.setOrigin(length*0.5f, thinkness*0.5f);
            rect.rotate(rotation * 180 / 3.1415926);
            rect.setFillColor(color);
            this->renderTexture.draw(rect);
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
            draw_line(vec3_vec2(rotate(cv::Vec3d(size, 0, 0), rot) + pos), vec3_vec2(rotate(cv::Vec3d(0, 0, 0), rot) + pos), CONSTANT::COLOR_RED_LIGHT, 2);
            draw_line(vec3_vec2(rotate(cv::Vec3d(0, size, 0), rot) + pos), vec3_vec2(rotate(cv::Vec3d(0, 0, 0), rot) + pos), CONSTANT::COLOR_GREEN_LIGHT, 2);
            draw_line(vec3_vec2(rotate(cv::Vec3d(0, 0, size), rot) + pos), vec3_vec2(rotate(cv::Vec3d(0, 0, 0), rot) + pos), CONSTANT::COLOR_BLUE_LIGHT, 2);
        }

        void draw_camera(Camera* cam, std::array<cv::Vec3d, CONSTANT::NB_JOINTS> rays)
        {
            cv::Vec3d pos = cam->getPosition();
            cv::Mat rot = cam->getRotation();
            float size = 0.4;
            sf::Vector2f p1 = vec3_vec2(rotate(cv::Vec3d( size*0.2, size*0.4, size*0.5), rot) + pos);
            sf::Vector2f p2 = vec3_vec2(rotate(cv::Vec3d( size*0.2,-size*0.4, size*0.5), rot) + pos);
            sf::Vector2f p3 = vec3_vec2(rotate(cv::Vec3d(-size*0.2,-size*0.4, size*0.5), rot) + pos);
            sf::Vector2f p4 = vec3_vec2(rotate(cv::Vec3d(-size*0.2, size*0.4, size*0.5), rot) + pos);
            draw_line(vec3_vec2(rotate(cv::Vec3d(0, 0, size), rot) + pos), vec3_vec2(pos), CONSTANT::COLOR_BLUE_LIGHT, 2);
            draw_line(p1, vec3_vec2(pos), CONSTANT::COLOR_ORANGE_LIGHT, 2);
            draw_line(p2, vec3_vec2(pos), CONSTANT::COLOR_ORANGE_LIGHT, 2);
            draw_line(p3, vec3_vec2(pos), CONSTANT::COLOR_ORANGE_LIGHT, 2);
            draw_line(p4, vec3_vec2(pos), CONSTANT::COLOR_ORANGE_LIGHT, 2);
            draw_line(p1, p4, CONSTANT::COLOR_ORANGE_LIGHT, 2);
            draw_line(p2, p1, CONSTANT::COLOR_ORANGE_LIGHT, 2);
            draw_line(p3, p2, CONSTANT::COLOR_ORANGE_LIGHT, 2);
            draw_line(p4, p3, CONSTANT::COLOR_ORANGE_LIGHT, 2);

            if (cam->isDebugMode())
            {
                for(int i = 0; i < CONSTANT::NB_CONNECTIONS; i++)
                    draw_line(
                        vec3_vec2(rays[CONSTANT::POSE_CONNECTIONS[i][0]] + pos),
                        vec3_vec2(rays[CONSTANT::POSE_CONNECTIONS[i][1]] + pos),
                        CONSTANT::COLOR_PURPLE_LIGHT
                    );
            }
        }

        void draw_body()
        {
            std::array<cv::Vec3d, CONSTANT::NB_JOINTS> points = this->bp->getBody();
            for(int i = 0; i < CONSTANT::NB_CONNECTIONS; i++)
                draw_line(
                    vec3_vec2(points[CONSTANT::POSE_CONNECTIONS[i][0]]),
                    vec3_vec2(points[CONSTANT::POSE_CONNECTIONS[i][1]]),
                    CONSTANT::COLOR_PURPLE_LIGHT, 4
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
            this->setClearColor(CONSTANT::COLOR_CLEAR);
            init();
        }

        View(sf::Vector2i pos, sf::Vector2i size, sf::Color clearColor = CONSTANT::COLOR_CLEAR)
        {
            this->setDimensions(pos.x, pos.y, size.x, size.y);
            this->setClearColor(clearColor);
            init();
        }

        void generateTexture()
        {
            clear_tex();
            draw_origin(cv::Vec3d(0, 0, 0), cv::Mat::eye(cv::Size(3, 3), CV_64F), 0.5);
            std::vector<Camera*> cams = this->bp->getCameras();
            for (int i = 0; i < cams.size(); i++)
                draw_camera(cams.at(i), this->bp->getCamRays(i));
            draw_body();
            apply_tex();
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
            this->rotation.y += dt * 0.5;
            if (clicked)
            {
                sf::Vector2i delta = mousePos - lastMousePos;
                this->rotation.x += delta.y * 0.01;
                this->rotation.y -= delta.x * 0.01;
            }
            apply_rotation();
            lastMousePos = mousePos;
        }

        sf::Sprite getSprite(float dt)
        {
            this->generateTexture();
            return this->sprite;
        }

        void setBodyPos(BodyPos* bodyPos)
        {
            this->bp = bodyPos;
        }

        ~View()
        {

        }

    };
}