#pragma once

#include "components/simple_scene.h"
#include "components/text_renderer.h"


namespace m1
{
    class Tema1 : public gfxc::SimpleScene
    {
    public:
        Tema1();
        ~Tema1();

        void Init() override;

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
        void StartDuck();
        void CheckEdges();
        glm::mat3 duck;
        glm::mat3 head;
        glm::mat3 body;
        glm::mat3 beak;
        glm::mat3 wing1;
        glm::mat3 wing2;
        glm::mat3 eye;
        glm::mat3 iris;
        glm::mat3 sun;
        glm::ivec2 resolution;
        gfxc::TextRenderer* text;
        float angularStep;
        float startX, startY;
        float currX, currY;
        float size;
        float angle;
        float w = 75;
        float h = 240;
        float centerX;
        float centerY;
        float xDownRight, xUpRight, xUpLeft, yDownRight, yUpRight, yUpLeft;
        float timeLimit = 0;
        int nrLifes = 3;
        int nrDucks = 0;
        float initialSpeed = 200;
        float speed = 200;
        float nrGuns = 3;
        bool killed = false;
        int dontKill = 0;
        int currScore = 0;
        int maxScore = 19;
        int subScoreX = 990;
        int subScoreY = 615;
        int wGun = 30;
        int hGun = 45;
        bool addScore = false;
    };
}   // namespace m1
