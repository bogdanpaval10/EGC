#include "lab_m1/Tema2/Tema2.h"

#include <vector>
#include <string>
#include <iostream>
#include <fstream>

#include "lab_m1/Tema2/transform3D.h"

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema2::Tema2()
{
}


Tema2::~Tema2()
{
}


void Tema2::InitRoad()
{
    ifstream fin;
    fin.open(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::FILES, "in.txt"), ios::in);

    double x, y;
    while (fin >> x >> y) {
        road.push_back({x, 0, y});
    }
    fin.close();

    vector<VertexFormat> vertices;
    vector<unsigned int> indices;
    
    for (int i = 0; i < road.size() - 2; i++) {
        glm::vec3 d1 = glm::vec3(road[i + 1][0], 0, road[i + 1][2]) - glm::vec3(road[i][0], 0, road[i][2]);
        glm::vec3 p1 = glm::normalize(glm::cross(d1, glm::vec3(0, 1, 0)));
        glm::vec3 r1 = glm::vec3(road[i][0], 0, road[i][2]) + glm::vec3(2.75, 2.75, 2.75) * p1;
        glm::vec3 a1 = glm::vec3(road[i][0], 0, road[i][2]) - glm::vec3(2.75, 2.75, 2.75) * p1;

        glm::vec3 d2 = glm::vec3(road[i + 2][0], 0, road[i + 2][2]) - glm::vec3(road[i + 1][0], 0, road[i + 1][2]);
        glm::vec3 p2 = glm::normalize(glm::cross(d2, glm::vec3(0, 1, 0)));
        glm::vec3 r2 = glm::vec3(road[i + 1][0], 0, road[i + 1][2]) + glm::vec3(2.75, 2.75, 2.75) * p2;
        glm::vec3 a2 = glm::vec3(road[i + 1][0], 0, road[i + 1][2]) - glm::vec3(2.75, 2.75, 2.75) * p2;

        if (i % 4 == 0) {
            vertices.push_back(VertexFormat(r1 + glm::vec3(0, 0.05, 0), glm::vec3(0, 0, 0))); // formare mesh drum
            vertices.push_back(VertexFormat(a1 + glm::vec3(0, 0.05, 0), glm::vec3(0, 0, 0)));
            vertices.push_back(VertexFormat(r2 + glm::vec3(0, 0.05, 0), glm::vec3(0, 0, 0)));
            vertices.push_back(VertexFormat(a2 + glm::vec3(0, 0.05, 0), glm::vec3(0, 0, 0)));

            indices.push_back(i);
            indices.push_back(i + 1);
            indices.push_back(i + 2);
            indices.push_back(i + 3);
        }

        roadTriangles.push_back(glm::vec2(r1[0], r1[2])); // salvare (x, z) varfuri triunghiuri
        roadTriangles.push_back(glm::vec2(a1[0], a1[2]));
        roadTriangles.push_back(glm::vec2(a2[0], a2[2]));

        roadTriangles.push_back(glm::vec2(r1[0], r1[2])); // A
        roadTriangles.push_back(glm::vec2(a2[0], a2[2])); // B
        roadTriangles.push_back(glm::vec2(r2[0], r2[2])); // C
    }

    meshes["road"] = new Mesh("road");
    meshes["road"]->SetDrawMode(GL_TRIANGLE_STRIP);
    meshes["road"]->InitFromData(vertices, indices);
}


void Tema2::Init()
{
    InitRoad();

    // Initialize tx, ty and tz (the translation steps)
    translateX = 0;
    translateY = 0;
    translateZ = 0;

    // Sets the resolution of the small viewport
    glm::ivec2 resolution = window->GetResolution();
    miniViewportArea = ViewportArea(50, 50, resolution.x / 5.f, resolution.y / 5.f);

    camera = new Camera();

    {
        vector<VertexFormat> vertices
        {
            VertexFormat(glm::vec3(0, 0, 0), glm::vec3(0, 0.7, 0.1)),
            VertexFormat(glm::vec3(0, 0, 2), glm::vec3(0, 0.7, 0.1)),
            VertexFormat(glm::vec3(2, 0, 2), glm::vec3(0, 0.7, 0.1)),
            VertexFormat(glm::vec3(2, 0, 0), glm::vec3(0, 0.7, 0.1)),
        };

        vector<unsigned int> indices =
        {
            0, 2, 1,
            2, 0, 3
        };

        meshes["grass"] = new Mesh("grass");
        meshes["grass"]->InitFromData(vertices, indices);
    }

    {
        vector<VertexFormat> vertices
        {
            VertexFormat(glm::vec3(-0.3, 0,  1), glm::vec3(0, 1, 1)),
            VertexFormat(glm::vec3(0.4, 0,  1), glm::vec3(0, 1, 1)),
            VertexFormat(glm::vec3(-0.3, 0.5,  1), glm::vec3(0, 1, 1)),
            VertexFormat(glm::vec3(0.4, 0.5,  1), glm::vec3(0, 1, 1)),
            VertexFormat(glm::vec3(-0.3, 0,  -1), glm::vec3(0, 1, 1)),
            VertexFormat(glm::vec3(0.4, 0,  -1), glm::vec3(0, 1, 1)),
            VertexFormat(glm::vec3(-0.3, 0.5,  -1), glm::vec3(0, 1, 1)),
            VertexFormat(glm::vec3(0.4, 0.5,  -1), glm::vec3(0, 1, 1))
        };

        vector<unsigned int> indices =
        {
            0, 1, 2,    // indices for first triangle
            1, 3, 2,    // indices for second triangle
            2, 3, 7,
            2, 7, 6,
            1, 7, 3,
            1, 5, 7,
            6, 7, 4,
            7, 5, 4,
            0, 4, 1,
            1, 4, 5,
            2, 6, 4,
            0, 2, 4
        };

        meshes["car"] = new Mesh("car");
        meshes["car"]->InitFromData(vertices, indices);
    }

    {
        vector<VertexFormat> vertices
        {
            VertexFormat(glm::vec3(0.2, 0,  0.2), glm::vec3(0.545, 0.271, 0.075)),
            VertexFormat(glm::vec3(0.5, 0,  0.2), glm::vec3(0.545, 0.271, 0.075)),
            VertexFormat(glm::vec3(0.2, 1.1,  0.2), glm::vec3(0.545, 0.271, 0.075)),
            VertexFormat(glm::vec3(0.5, 1.1,  0.2), glm::vec3(0.545, 0.271, 0.075)),
            VertexFormat(glm::vec3(0.2, 0,  -0.2), glm::vec3(0.545, 0.271, 0.075)),
            VertexFormat(glm::vec3(0.5, 0,  -0.2), glm::vec3(0.545, 0.271, 0.075)),
            VertexFormat(glm::vec3(0.2, 1.1,  -0.2), glm::vec3(0.545, 0.271, 0.075)),
            VertexFormat(glm::vec3(0.5, 1.1,  -0.2), glm::vec3(0.545, 0.271, 0.075))
        };

        vector<unsigned int> indices =
        {
            0, 1, 2,    // indices for first triangle
            1, 3, 2,    // indices for second triangle
            2, 3, 7,
            2, 7, 6,
            1, 7, 3,
            1, 5, 7,
            6, 7, 4,
            7, 5, 4,
            0, 4, 1,
            1, 4, 5,
            2, 6, 4,
            0, 2, 4
        };

        meshes["trunk"] = new Mesh("trunk");
        meshes["trunk"]->InitFromData(vertices, indices);
    }

    {
        vector<VertexFormat> vertices
        {
            VertexFormat(glm::vec3(-0.1, 1.1,  0.5), glm::vec3(0.000, 0.392, 0.000)),
            VertexFormat(glm::vec3(0.8, 1.1,  0.5), glm::vec3(0.000, 0.392, 0.000)),
            VertexFormat(glm::vec3(-0.1, 1.9,  0.5), glm::vec3(0.000, 0.392, 0.000)),
            VertexFormat(glm::vec3(0.8, 1.9,  0.5), glm::vec3(0.000, 0.392, 0.000)),
            VertexFormat(glm::vec3(-0.1, 1.1,  -0.5), glm::vec3(0.000, 0.392, 0.000)),
            VertexFormat(glm::vec3(0.8, 1.1,  -0.5), glm::vec3(0.000, 0.392, 0.000)),
            VertexFormat(glm::vec3(-0.1, 1.9,  -0.5), glm::vec3(0.000, 0.392, 0.000)),
            VertexFormat(glm::vec3(0.8, 1.9,  -0.5), glm::vec3(0.000, 0.392, 0.000))
        };

        vector<unsigned int> indices =
        {
            0, 1, 2,    // indices for first triangle
            1, 3, 2,    // indices for second triangle
            2, 3, 7,
            2, 7, 6,
            1, 7, 3,
            1, 5, 7,
            6, 7, 4,
            7, 5, 4,
            0, 4, 1,
            1, 4, 5,
            2, 6, 4,
            0, 2, 4
        };

        meshes["crown"] = new Mesh("crown");
        meshes["crown"]->InitFromData(vertices, indices);
    }

    // TODO(student): After you implement the changing of the projection
    // parameters, remove hardcodings of these parameters
    projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);
}


void Tema2::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0.5, 0.8, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


void Tema2::GenerateTrees()
{
    int offsetX, offsetZ, offsetI = 4;
    for (int i = 0; i < road.size() - 5; i += offsetI) { // randare copaci exterior
        if (i >= 0 && i < 75) {
            offsetX = 6;
            offsetZ = 1;
            offsetI = 4;
        }
        if (i >= 75 && i < 155) {
            offsetX = 2;
            offsetZ = -8;
            offsetI = 3;
        }
        if (i >= 155 && i < 235) {
            offsetX = -8;
            offsetZ = 4;
            offsetI = 2;
        }
        if (i >= 235) {
            offsetX = 5;
            offsetZ = 0;
            offsetI = 2;
        }
        glm::mat4 tr = glm::mat4(1);
        tr *= transform3D::Translate(road[i][0] + offsetX, 0, road[i][2] + offsetZ);
        RenderMesh(meshes["trunk"], shaders["VertexColor"], tr);
        glm::mat4 cor = glm::mat4(1);
        cor *= transform3D::Translate(road[i][0] + offsetX, 0, road[i][2] + offsetZ);
        RenderMesh(meshes["crown"], shaders["VertexColor"], cor);
    }

    offsetI = 4;
    for (int i = 0; i < road.size() - 3; i += offsetI) { // randare copaci interior
        if (i >= 0 && i < 90) {
            offsetX = -7;
            offsetZ = 0;
            offsetI = 4;
        }
        if (i >= 90 && i < 135) {
            offsetX = -3;
            offsetZ = 9;
            offsetI = 4;
        }

        if (i >= 135 && i < 220) {
            offsetX = 7;
            offsetZ = 0;
            offsetI = 4;
        }
        if (i >= 220 && i < 240) {
            continue;
        }
        if (i >= 240) {
            offsetX = -6;
            offsetZ = 0;
            offsetI = 3;
        }
        glm::mat4 tr = glm::mat4(1);
        tr *= transform3D::Translate(road[i][0] + offsetX, 0, road[i][2] + offsetZ);
        RenderMesh(meshes["trunk"], shaders["VertexColor"], tr);
        glm::mat4 cor = glm::mat4(1);
        cor *= transform3D::Translate(road[i][0] + offsetX, 0, road[i][2] + offsetZ);
        RenderMesh(meshes["crown"], shaders["VertexColor"], cor);
    }
}


void Tema2::Update(float deltaTimeSeconds)
{
    projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);
    camera->Set(glm::vec3(0, 1.5f, 3.5f), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
    camera->MoveForward(-translateZ);
    camera->TranslateRight(translateX);
    camera->MoveForward(3.5f);
    camera->RotateFirstPerson_OY(angle);
    camera->MoveForward(-3.5f);
    
    RenderMesh(meshes["road"], shaders["VertexColor"], glm::mat4(1));
    GenerateTrees();
    
    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(translateX, translateY + 0.05f, translateZ);
    modelMatrix *= transform3D::RotateOY(angle);
    RenderMesh(meshes["car"], shaders["VertexColor"], modelMatrix);

    grass = glm::mat4(1);
    grass = glm::translate(grass, glm::vec3(-100.0f, 0.0f, -100.0f));
    grass = glm::scale(grass, glm::vec3(100.0f));
    RenderMesh(meshes["grass"], shaders["VertexColor"], grass);


    glClear(GL_DEPTH_BUFFER_BIT);
    glViewport(miniViewportArea.x, miniViewportArea.y, miniViewportArea.width, miniViewportArea.height);

    projectionMatrix = glm::ortho(left, right, bottom, top, zNear, zFar);
    camera->Set(glm::vec3(translateX, 15.5f, translateZ), glm::vec3(translateX, 0, translateZ), glm::vec3(-1, 0, 1));

    RenderMesh(meshes["road"], shaders["VertexColor"], glm::mat4(1));
    GenerateTrees();

    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(translateX, translateY, translateZ);
    modelMatrix *= transform3D::RotateOY(angle);
    RenderMesh(meshes["car"], shaders["VertexColor"], modelMatrix);

    grass = glm::mat4(1);
    grass = glm::translate(grass, glm::vec3(-100.0f, 0.0f, -100.0f));
    grass = glm::scale(grass, glm::vec3(100.0f));
    RenderMesh(meshes["grass"], shaders["VertexColor"], grass);
}


void Tema2::FrameEnd()
{
}


void Tema2::RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix)
{
    if (!mesh || !shader || !shader->program)
        return;

    // Render an object using the specified shader and the specified position
    shader->Use();
    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    mesh->Render();
}


double Tema2::TriangleArea(double currAX, double currAZ, double currBX, double currBZ, double currCX, double currCZ)
{
    double abX = currBX - currAX;
    double abY = currBZ - currAZ;
    double acX = currCX - currAX;
    double acY = currCZ - currAZ;
    double cross_prod = abX * acY - abY * acX;

    return fabs(cross_prod) / 2;
}


bool Tema2::InsideTriangle(double currX, double currZ, double currAX, double currAZ, double currBX, double currBZ, double currCX, double currCZ)
{
    double triangleArea = TriangleArea(currAX, currAZ, currBX, currBZ, currCX, currCZ);
    double areaSum = TriangleArea(currAX, currAZ, currBX, currBZ, currX, currZ)
        + TriangleArea(currAX, currAZ, currCX, currCZ, currX, currZ)
        + TriangleArea(currBX, currBZ, currCX, currCZ, currX, currZ);

    if (triangleArea == areaSum) {
        return true;
    }
    return false;
}


void Tema2::OnInputUpdate(float deltaTime, int mods)
{
    int ok = 0;
    if (window->KeyHold(GLFW_KEY_W)) {
        translateX -= speed * sin(angle) * deltaTime;
        translateZ -= speed * cos(angle) * deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_S)) {
        translateX += speed * sin(angle) * deltaTime;
        translateZ += speed * cos(angle) * deltaTime;
    }

    for (int i = 0; i < roadTriangles.size(); i += 3) {
        if (InsideTriangle(translateX, translateZ, roadTriangles[i][0], roadTriangles[i][1], roadTriangles[i + 1][0], roadTriangles[i + 1][1], roadTriangles[i + 2][0], roadTriangles[i + 2][1]) == true) {
            ok = 1;
            break; // daca este intr-un triunghi de pe drum
        }
    }

    if (ok == 0) { // daca nu este in niciun triunghi (este in afara drumului)
        if (window->KeyHold(GLFW_KEY_W)) { // nu se mai deplaseaza masina
            translateX += speed * sin(angle) * deltaTime;
            translateZ += speed * cos(angle) * deltaTime;
        }
        if (window->KeyHold(GLFW_KEY_S)) {
            translateX -= speed * sin(angle) * deltaTime;
            translateZ -= speed * cos(angle) * deltaTime;
        }
    }

    if (window->KeyHold(GLFW_KEY_W)) {
        if (window->KeyHold(GLFW_KEY_D)) {
            angle -= deltaTime;
        }
        if (window->KeyHold(GLFW_KEY_A)) {
            angle += deltaTime;
        }
    }
    if (window->KeyHold(GLFW_KEY_S)) {
        if (window->KeyHold(GLFW_KEY_D)) {
            angle += deltaTime;
        }
        if (window->KeyHold(GLFW_KEY_A)) {
            angle -= deltaTime;
        }
    }
}


void Tema2::OnKeyPress(int key, int mods)
{
    // Add key press event
}


void Tema2::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetZ)
{
}


void Tema2::OnWindowResize(int width, int height)
{
}
