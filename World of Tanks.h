#pragma once

#include "components/transform.h"
#include "Tank.h"
#include "Collisions.h"
#include "object3D.h"


namespace m1
{
    class WOT : public gfxc::SimpleScene
    {
     public:
        WOT();
        ~WOT();

        void Init() override;

        struct Building {
            glm::vec3 position;
            glm::vec3 scale;
        };

     private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

     protected:
        cam::Camera *camera;
        glm::mat4 projectionMatrix;
        std::vector<Projectile> projectiles;
        Tank* tanks[5];
        float bodyChange[5] = {};
        Tank* mainTank;
        Building* buildings;
    };
}   // namespace m1
