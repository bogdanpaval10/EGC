#include "lab_m1/tema1/tema1.h"

#include <vector>
#include <iostream>


#include "lab_m1/Tema1/transform2D.h"
#include "Shapes.h"

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema1::Tema1()
{
}


Tema1::~Tema1()
{
}


void Tema1::StartDuck()
{
    // initializeaza parametrii pentru o noua rata
    startX = 100 + rand() % 600;
    startY = 30;
    currX = 10 + startX;
    currY = 10 + startY;
    angle = (20 + rand() % 70) * (M_PI / 180);
    xDownRight = currX + w;
    yDownRight = currY;
    xUpRight = currX + w;
    yUpRight = currY + h;
    xUpLeft = currX;
    yUpLeft = currY + h;
}


void Tema1::Init()
{
    resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    glm::vec3 corner = glm::vec3(0, 0, 0);

    StartDuck();

    size = 72;

    text = new gfxc::TextRenderer(window->props.selfDir, window->GetResolution().x, window->GetResolution().y);
    text->Load(window->props.selfDir + "\\assets\\fonts\\Hack-Bold.ttf", 128);

    // Initialize angularStep
    angularStep = 1.4;

    Mesh* body = Shapes::CreateTriangle("body", corner, size, glm::vec3(0.545, 0.271, 0.075), true);
    AddMeshToList(body);

    Mesh* wing1 = Shapes::CreateWings("wing1", corner, size / 2, glm::vec3(0.545, 0.271, 0.075), true);
    AddMeshToList(wing1);

    Mesh* wing2 = Shapes::CreateWings("wing2", corner, size / 2, glm::vec3(0.545, 0.271, 0.075), true);
    AddMeshToList(wing2);

    Mesh* head = Shapes::CreateCircle("head", corner + glm::vec3(0, 10, 0), size * 0.56, glm::vec3(0.000, 0.392, 0.000), true);
    AddMeshToList(head);

    Mesh* eye = Shapes::CreateCircle("eye", corner, 8, glm::vec3(1.000, 1.000, 1.000), true);
    AddMeshToList(eye);

    Mesh* iris = Shapes::CreateCircle("iris", corner, 3, glm::vec3(0.000, 0.000, 0.000), true);
    AddMeshToList(iris);

    Mesh* beak = Shapes::CreateTriangle("beak", corner, size / 4, glm::vec3(0.855, 0.647, 0.125), true);
    AddMeshToList(beak);

    Mesh* life = Shapes::CreateCircle("life", corner, 20, glm::vec3(1.000, 0.000, 0.000), true);
    AddMeshToList(life);

    Mesh* gun = Shapes::CreateRectangle("gun", corner, wGun, hGun, glm::vec3(0.196, 0.804, 0.196), true);
    AddMeshToList(gun);

    Mesh* scoreEdge = Shapes::CreateRectangle("scoreEdge", corner + glm::vec3(990, 615, 0), 246, 40, glm::vec3(1.000, 1.000, 1.000));
    AddMeshToList(scoreEdge);

    Mesh* oneScore = Shapes::CreateRectangle("oneScore", corner, 30, 40, glm::vec3(0.000, 0.000, 1.000), true);
    AddMeshToList(oneScore);

    Mesh* grassBack = Shapes::CreateRectangle("grassBack", corner, resolution.x + 100, 150, glm::vec3(0.000, 0.392, 0.000), true);
    AddMeshToList(grassBack);

    Mesh* grass = Shapes::CreateRectangle("grass", corner, resolution.x, 150, glm::vec3(0.000, 0.502, 0.000), true);
    AddMeshToList(grass);

    Mesh* sun = Shapes::CreateCircle("sun", corner, 60, glm::vec3(1.000, 0.843, 0.000), true);
    AddMeshToList(sun);
}


void Tema1::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0.125, 0.698, 0.667, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}

void Tema1::CheckEdges()
{
    centerX = startX + w / 2; // coordonate centru dreptunghi incadrare rata
    centerY = startY + h / 2;

    if (currY < startY - centerY) { // s-a lovit de partea de jos
        angle = angle * (-1);
    }
    if (currY > resolution.y + startY - centerY) { // s-a lovit de partea de sus
        angle = angle * (-1);
    }

    if (currX < startX - centerX) { // s-a lovit de partea stanga
        angle = M_PI - angle; // 180 - angle
    }
    if (currX > resolution.x + startX - centerX) { // s-a lovit de partea dreapta
        angle = M_PI - angle; // 180 - angle
    }
}


void Tema1::Update(float deltaTimeSeconds)
{
    duck = glm::mat3(1);
    timeLimit += deltaTimeSeconds;

    text->RenderText("Level " + to_string(1 + nrDucks / 5), 1040, 140, 0.30);


    if (nrDucks % 5 == 0 && nrDucks != 0) { // creste viteza dupa fiecare 5 rate
        speed = initialSpeed + (nrDucks / 2) * initialSpeed / 5;
    }

    currX += cos(angle) * deltaTimeSeconds * speed; // deplasare varfuri dreptunghi rata
    currY += sin(angle) * deltaTimeSeconds * speed; // in functie de unghiul de rotatie
    xDownRight += cos(angle) * deltaTimeSeconds * speed;
    yDownRight += sin(angle) * deltaTimeSeconds * speed;
    xUpRight += cos(angle) * deltaTimeSeconds * speed;
    yUpRight += sin(angle) * deltaTimeSeconds * speed;
    xUpLeft += cos(angle) * deltaTimeSeconds * speed;
    yUpLeft += sin(angle) * deltaTimeSeconds * speed;

    CheckEdges(); // verifica daca a ajuns la vreun perete

    if (killed == true) { // daca a fost impuscata, moare
        angle = -M_PI_2;
        dontKill = 1;
        if (currY < -100 && nrLifes > 0) { // dupa ce a iesit din ecran, se genereaza o noua rata, daca mai sunt vieti
            // initializeaza parametrii pentru o noua rata
            startX = 100 + rand() % 600;
            startY = 30;
            currX = 10 + startX;
            currY = 10 + startY;
            angle = (20 + rand() % 70) * (M_PI / 180);
            xDownRight = currX + w;
            yDownRight = currY;
            xUpRight = currX + w;
            yUpRight = currY + h;
            xUpLeft = currX;
            yUpLeft = currY + h;

            timeLimit = 0;
            nrDucks++;
            dontKill = 0;
            nrGuns = 3;
            currScore++;
            killed = false;
        }
    }

    if ((timeLimit > 10 || nrGuns == 0) && killed == false) { // daca s-a depasit timpul unei rate de a sta pe ecran
        angle = M_PI_2;                                           // sau nu mai are gloante, atunci evadeaza
        dontKill = 1;
        if (currY > 720 && nrLifes > 0) { // dupa ce a iesit din ecran, se genereaza o noua rata, daca mai sunt vieti
            // initializeaza parametrii pentru o noua rata
            startX = 100 + rand() % 600;
            startY = 30;
            currX = 10 + startX;
            currY = 10 + startY;
            angle = (20 + rand() % 70) * (M_PI / 180);
            xDownRight = currX + w;
            yDownRight = currY;
            xUpRight = currX + w;
            yUpRight = currY + h;
            xUpLeft = currX;
            yUpLeft = currY + h;

            timeLimit = 0;
            nrDucks++;
            nrGuns = 3;
            dontKill = 0;
            nrLifes--;
        }
    }


    angularStep += deltaTimeSeconds / 2;
    if (angularStep > 1.4) { // viteza aripi
        angularStep = angularStep / 2;
    }


    glm::mat3 life1 = glm::mat3(1);
    if (nrLifes >= 1) {
        life1 *= transform2D::Translate(60, 720 - 50);
        RenderMesh2D(meshes["life"], shaders["VertexColor"], life1);
    }

    glm::mat3 life2 = glm::mat3(1);
    if (nrLifes >= 2) {
        life2 *= transform2D::Translate(120, 720 - 50);
        RenderMesh2D(meshes["life"], shaders["VertexColor"], life2);
    }

    glm::mat3 life3 = glm::mat3(1);
    if (nrLifes >= 3) {
        life3 *= transform2D::Translate(180, 720 - 50);
        RenderMesh2D(meshes["life"], shaders["VertexColor"], life3);
    }

    if (nrLifes == 2) { // se scoate viata 3
        life3 *= transform2D::Translate(60, 720 + 30);
    }
    if (nrLifes == 1) { // se scoate viata 2
        life2 *= transform2D::Translate(60, 720 + 30);
    }
    if (nrLifes == 0) { // se scoate viata 1
        life1 *= transform2D::Translate(60, 720 + 30);
    }


    glm::mat3 scoreEdge = glm::mat3(1); // chenar scor
    RenderMesh2D(meshes["scoreEdge"], shaders["VertexColor"], scoreEdge);

    glm::mat3 grass = glm::mat3(1); // iarba deschisa la culoare
    RenderMesh2D(meshes["grass"], shaders["VertexColor"], grass);

    glm::mat3 grassBack = glm::mat3(1); // iarba inchisa la culoare
    grassBack *= transform2D::Translate(0, 10);
    grassBack *= transform2D::Rotate(0.09);
    RenderMesh2D(meshes["grassBack"], shaders["VertexColor"], grassBack);


    for (int i = 0; i < currScore && i < maxScore; i++) { // se adauga un patratel la scor
        glm::mat3 subScore = glm::mat3(1);
        subScore *= transform2D::Translate(subScoreX + i * 12, subScoreY);
        RenderMesh2D(meshes["oneScore"], shaders["VertexColor"], subScore);
    }

    if (nrLifes > 0) { // daca mai sunt vieti, se poate randa rata
        glm::mat3 gun1 = glm::mat3(1);
        if (nrGuns >= 1) {
            gun1 *= transform2D::Translate(hGun, 720 - 140);
            RenderMesh2D(meshes["gun"], shaders["VertexColor"], gun1);
        }

        glm::mat3 gun2 = glm::mat3(1);
        if (nrGuns >= 2) {
            gun2 *= transform2D::Translate(hGun + 60, 720 - 140);
            RenderMesh2D(meshes["gun"], shaders["VertexColor"], gun2);
        }

        glm::mat3 gun3 = glm::mat3(1);
        if (nrGuns >= 3) {
            gun3 *= transform2D::Translate(hGun + 120, 720 - 140);
            RenderMesh2D(meshes["gun"], shaders["VertexColor"], gun3);
        }

        if (nrGuns == 2) { // se scoate glontul 3
            gun3 *= transform2D::Translate(hGun, 720 + 50);
        }
        if (nrGuns == 1) { // se scoate glontul 2
            gun2 *= transform2D::Translate(hGun, 720 + 50);
        }
        if (nrGuns == 0) { // se scoate glontul 1
            gun1 *= transform2D::Translate(hGun, 720 + 50);
        }

        duck *= transform2D::Translate(currX, currY); // deplaseaza rata
        duck *= transform2D::Translate(centerX - startX, centerY - startY);
        duck *= transform2D::Rotate(-M_PI_2 + angle); // roteste rata
        duck *= transform2D::Translate(startX - centerX, startY - centerY);


        body = duck; // de aici body este matricea de baza

        iris = body;
        iris *= transform2D::Translate(size / 2, size * 2.5);
        RenderMesh2D(meshes["iris"], shaders["VertexColor"], iris);

        eye = body;
        eye *= transform2D::Translate(size / 2, size * 2.5);
        RenderMesh2D(meshes["eye"], shaders["VertexColor"], eye);

        head = body;
        head *= transform2D::Translate(size / 2, size * 2.1);
        RenderMesh2D(meshes["head"], shaders["VertexColor"], head);

        RenderMesh2D(meshes["body"], shaders["VertexColor"], body);

        beak = body;
        beak *= transform2D::Translate(size * 0.35, size * 2.75);
        RenderMesh2D(meshes["beak"], shaders["VertexColor"], beak);

        wing1 = body;
        wing1 *= transform2D::Translate(size * 0.35, size * 0.55);
        wing1 *= transform2D::Rotate(angularStep);
        RenderMesh2D(meshes["wing1"], shaders["VertexColor"], wing1);

        wing2 = body;
        wing2 *= transform2D::Translate(size * 0.40, size * 0.95);
        wing2 *= transform2D::Rotate(-angularStep);
        RenderMesh2D(meshes["wing2"], shaders["VertexColor"], wing2);

        glm::mat3 sun = glm::mat3(1);
        sun *= transform2D::Translate(1280 - 35, 720);
        RenderMesh2D(meshes["sun"], shaders["VertexColor"], sun);

        text->RenderText("Duck", 360, 80, 1.7, glm::vec3(1.000, 0.843, 0.000));
        text->RenderText("Hunter", 240, 270, 1.7, glm::vec3(1.000, 0.843, 0.000));
    }
    else {
        text->RenderText("Game Over", 340, 270, 1);
        text->RenderText("Final Score: " + to_string(currScore), 410, 390, 0.5);
    }
}


void Tema1::FrameEnd()
{
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema1::OnInputUpdate(float deltaTime, int mods)
{
}


void Tema1::OnKeyPress(int key, int mods)
{
    // Add key press event
}


void Tema1::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event

}


void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
    mouseY = 720 - mouseY;
    int inside = 0;
    if (mouseX >= currX - w && mouseX <= xUpRight + w && mouseY >= yDownRight && mouseY <= yUpLeft) {
        inside = 1;
    }
    if (mouseX >= xUpLeft && mouseX <= xDownRight && mouseY >= currY && mouseY <= yUpRight) {
        inside = 1;
    }
    if (mouseX >= xDownRight && mouseX <= xUpLeft && mouseY >= yUpRight && mouseY <= currY) {
        inside = 1;
    }
    if (mouseX >= xUpRight && mouseX <= currX && mouseY >= yUpLeft && mouseY <= yDownRight) {
        inside = 1;
    }

    if (dontKill == 0) { // daca se poate sa fie omorata
        nrGuns--;
        if (inside == 1) {
            killed = true;
        }
    }
}


void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema1::OnWindowResize(int width, int height)
{
}
