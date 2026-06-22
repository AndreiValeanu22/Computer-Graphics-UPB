#pragma once

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"

#include <string>
#include <vector>

namespace object3D {
    Mesh* CreateTerrain(const std::string& name,
                        unsigned int rows,
                        unsigned int cols,
                        float cellSize,
                        const std::vector<std::vector<glm::vec3>>& colors);

    Mesh* CreateParallelepiped(const std::string& name,
                               glm::vec3 dimensions,
                               glm::vec3 color);

    Mesh* CreateCylinder(const std::string& name,
                         float radius,
                         float height,
                         glm::vec3 color);

    Mesh* CreatePyramid(const std::string& name,
                        float base,
                        float height,
                        glm::vec3 color);

    Mesh* CreateRailNormalStraight(const std::string& name, float tileSize);
    Mesh* CreateRailNormalCorner(const std::string& name, float tileSize);

    Mesh* CreateRailBridgeStraight(const std::string& name, float tileSize);
    Mesh* CreateRailBridgeCorner(const std::string& name, float tileSize);

    Mesh* CreateRailTunnelStraight(const std::string& name, float tileSize);
    Mesh* CreateRailTunnelCorner(const std::string& name, float tileSize);
}
