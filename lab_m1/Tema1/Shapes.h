#pragma once

#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"


namespace Shapes
{
    Mesh* CreateTriangle(const std::string &name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill = false);
    Mesh* CreateRectangle(const std::string &name, glm::vec3 leftBottomCorner, float length, float L, glm::vec3 color, bool fill = false);
    Mesh* CreateCircle(const std::string &name, glm::vec3 corner, int radius, glm::vec3 color, bool fill = false);
    Mesh* CreateWings(const std::string &name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill = false);
}
