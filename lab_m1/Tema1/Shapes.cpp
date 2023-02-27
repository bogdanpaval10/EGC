#include "Shapes.h"

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"

Mesh* Shapes::CreateTriangle(
    const std::string &name,
    glm::vec3 corner,
    float length,
    glm::vec3 color,
    bool fill)
{

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(length, 0, 0), color),
        VertexFormat(corner + glm::vec3(length / 2.1, length * 2.1, 0), color),
        VertexFormat(corner + glm::vec3(0, 0, 0), color)
    };

    Mesh* triangle = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2 };

    if (!fill) {
        triangle->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        indices.push_back(0);
        indices.push_back(2);
    }

    triangle->InitFromData(vertices, indices);
    return triangle;
}

Mesh* Shapes::CreateRectangle(
    const std::string &name,
    glm::vec3 corner,
    float length,
    float L,
    glm::vec3 color,
    bool fill)
{

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(length, 0, 0), color),
        VertexFormat(corner + glm::vec3(length, L, 0), color),
        VertexFormat(corner + glm::vec3(0, L, 0), color)
    };

    Mesh* rectangle = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        rectangle->SetDrawMode(GL_LINE_LOOP);
    } else {
        indices.push_back(0);
        indices.push_back(2);
    }

    rectangle->InitFromData(vertices, indices);
    return rectangle;
}

Mesh* Shapes::CreateCircle(
    const std::string& name,
    glm::vec3 corner,
    int radius,
    glm::vec3 color,
    bool fill)
{

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(glm::vec3(corner[0], corner[1], 0), color)
    };

    std::vector<unsigned int> indices;

    vertices.push_back(VertexFormat(glm::vec3(corner[0] + (radius * cos(0)), corner[1] + (radius * sin(0)), 0), color));

    for (int i = 1; i < 360; i++) {
        vertices.push_back(VertexFormat(glm::vec3(corner[0] + (radius * cos(i * 2 * M_PI / 360)), corner[1] + (radius * sin(i * 2 * M_PI / 360)), 0), color));
        indices.push_back(0);
        indices.push_back(i);
        indices.push_back(i + 1);
    }

    indices.push_back(0);
    indices.push_back(360);
    indices.push_back(1);

    Mesh* circle = new Mesh(name);

    if (!fill) {
        circle->SetDrawMode(GL_LINE_LOOP);
    }

    circle->InitFromData(vertices, indices);
    return circle;
}

Mesh* Shapes::CreateWings(
    const std::string& name,
    glm::vec3 corner,
    float length,
    glm::vec3 color,
    bool fill)
{

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(length / 2, length * 2, 0), color),
        VertexFormat(corner + glm::vec3(length, 0, 0), color),
        VertexFormat(corner + glm::vec3(0, 0, 0), color),
    };

    Mesh* wing = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2 };

    if (!fill) {
        wing->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        indices.push_back(0);
        indices.push_back(2);
    }

    wing->InitFromData(vertices, indices);
    return wing;
}
