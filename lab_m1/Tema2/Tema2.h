#pragma once

#include "components/simple_scene.h"
#include "lab_m1/Tema2/lab_camera.h"


namespace m1
{
    class Tema2 : public gfxc::SimpleScene
    {
     public:
        struct ViewportArea
        {
            ViewportArea() : x(0), y(0), width(1), height(1) {}
            ViewportArea(int x, int y, int width, int height)
                : x(x), y(y), width(width), height(height) {}
            int x;
            int y;
            int width;
            int height;
        };

        Tema2();
        ~Tema2();

        void Init() override;

     private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;
        void RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix) override;
        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetZ) override;
        void OnWindowResize(int width, int height) override;
        void InitRoad();
        void GenerateTrees();
        double TriangleArea(double currAX, double currAZ, double currBX, double currBZ, double currCX, double currCZ);
        bool InsideTriangle(double currX, double currZ, double currAX, double currAZ, double currBX, double currBZ, double currCX, double currCZ);

     protected:
        std::vector<glm::vec3> road;
        std::vector<glm::vec2> roadTriangles;
        glm::mat4 modelMatrix;
        glm::mat4 grass;
        Camera* camera;
        glm::mat4 projectionMatrix;
        float translateX, translateY, translateZ;
        ViewportArea miniViewportArea;
        float speed = 5;
        float angle = RADIANS(-30);
        float right = -40, left = 40, top = -20, bottom = 20;
        float zNear = 0.01f, zFar = 200.f;
    };
}   // namespace m1

