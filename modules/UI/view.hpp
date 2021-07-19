#pragma once
#include "../constants.hpp"
#include "graphicElement.hpp"
#include "../engine/camera.hpp"

namespace owo
{
    class View : public virtual GraphicElement
    {
    private:
        float camDist;
        sf::Vector2f rotation;
        
        Vec3 camPos;
        Vec3 camRot;
        float camFOV;

        sf::Vector2i lastMousePos;

        Camera* camObj;

        void init()
        {
            this->renderTexture.create(this->dimensions.width, this->dimensions.height);
            this->sprite.setPosition(this->dimensions.left, this->dimensions.top);
            this->camDist = 4;
            camFOV = 90;
            apply_rotation();
        }

        void apply_rotation()
        {
            this->camRot.x = this->rotation.x;
            this->camRot.y = this->rotation.y;
            this->camPos.x = std::cos(this->rotation.x) * -camDist * std::sin(this->rotation.y);
            this->camPos.y = std::sin(this->rotation.x) *  camDist;
            this->camPos.z = std::cos(this->rotation.x) *  camDist * std::cos(this->rotation.y);
        }

        sf::Vector2f vec3_vec2(Vec3 coord)
        {
            if (coord.z - camPos.z == 0)
                return sf::Vector2f();
            float distZ = coord.z-camPos.z;
            float distY = coord.y-camPos.y;
            float distX = coord.x-camPos.x;
            float sinRotY = std::sin(camRot.y);
            float sinRotX = std::sin(camRot.x);
            float cosRotY = std::cos(camRot.y);
            float cosRotX = std::cos(camRot.x);
            float Zresult = (distZ * cosRotY) - (distX * sinRotY);
            float Zresult2 = (distY * sinRotX) + (Zresult * cosRotX);
            float Xresult = (distZ * sinRotY) + (distX * cosRotY);
            float Yresult = (distY * cosRotX) - (Zresult * sinRotX);
            if (Zresult2 > 0)
                return sf::Vector2f();
            return sf::Vector2f(camFOV * (Xresult / Zresult2) + this->dimensions.width/2, camFOV * (Yresult / Zresult2) + this->dimensions.height/2);
        }

        void clear_tex()
        {
            this->renderTexture.clear(CONST::COLOR_CLEAR);
        }

        void draw_line(sf::Vector2f p1, sf::Vector2f p2, sf::Color color = CONST::COLOR_FOREGROUND)
        {
            sf::Vertex line[] = {sf::Vertex(p1, color), sf::Vertex(p2, color)};
            this->renderTexture.draw(line, 2, sf::Lines);
        }

        void draw_origin(float x, float y, float z, float size = 1.f)
        {
            draw_line(vec3_vec2(Vec3(x+size, y+0, z+0)), vec3_vec2(Vec3(x-size, y+0, z+0)), CONST::COLOR_RED_LIGHT);
            draw_line(vec3_vec2(Vec3(x+0, y+size, z+0)), vec3_vec2(Vec3(x+0, y-size, z+0)), CONST::COLOR_GREEN_LIGHT);
            draw_line(vec3_vec2(Vec3(x+0, y+0, z+size)), vec3_vec2(Vec3(x+0, y+0, z-size)), CONST::COLOR_BLUE_LIGHT);
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
            this->rotation.y -= dt * 0.5;
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
            clear_tex();
            draw_origin(0, 0, 0);
            Vec3 pos = camObj->getPosition();
            draw_origin(pos.x, pos.y, pos.z, 0.4);
            apply_tex();
            return this->sprite;
        }

        void setCameraObj(Camera* cam)
        {
            this->camObj = cam;
        }

        ~View()
        {

        }

    };
}