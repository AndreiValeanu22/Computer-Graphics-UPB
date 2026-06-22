#include "object3D.h"

const float kPi = std::acos(-1.0f); // Linia asta defineste constanta Pi

namespace {
    void AppendBox(std::vector<VertexFormat>& outV,
                   std::vector<unsigned int>& outI,
                   const glm::vec3& dimensions,
                   const glm::vec3& color,
                   const glm::vec3& translate) {
        const glm::vec3 half = dimensions * 0.5f;
        const auto offset = static_cast<unsigned int>(outV.size());

        outV.emplace_back(glm::vec3(-half.x, -half.y, -half.z) + translate, color);
        outV.emplace_back(glm::vec3(half.x, -half.y, -half.z) + translate, color);
        outV.emplace_back(glm::vec3(half.x, -half.y, half.z) + translate, color);
        outV.emplace_back(glm::vec3(-half.x, -half.y, half.z) + translate, color);

        outV.emplace_back(glm::vec3(-half.x, half.y, -half.z) + translate, color);
        outV.emplace_back(glm::vec3(half.x, half.y, -half.z) + translate, color);
        outV.emplace_back(glm::vec3(half.x, half.y, half.z) + translate, color);
        outV.emplace_back(glm::vec3(-half.x, half.y, half.z) + translate, color);

        for (unsigned int i = 0; i < 36; i++) {
            const unsigned int idx[] = {
                0, 1, 2, 2, 3, 0,
                4, 5, 6, 6, 7, 4,
                0, 1, 5, 5, 4, 0,
                2, 3, 7, 7, 6, 2,
                0, 3, 7, 7, 4, 0,
                1, 2, 6, 6, 5, 1
            };

            outI.push_back(idx[i] + offset);
        }
    }
}

Mesh* object3D::CreateTerrain(const std::string& name, // creeaza un teren dreptunghiular format din celule colorate
                              const unsigned int rows,
                              const unsigned int cols,
                              const float cellSize,
                              const std::vector<std::vector<glm::vec3>>& colors) {
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    for (unsigned int row = 0; row < rows; row++) {
        for (unsigned int col = 0; col < cols; col++) {
            const glm::vec3 color = colors[row][col];
            const float x = static_cast<float>(col) * cellSize;
            const float z = static_cast<float>(row) * cellSize;

            vertices.emplace_back(glm::vec3(x, 0, z), color);
            vertices.emplace_back(glm::vec3(x + cellSize, 0, z), color);
            vertices.emplace_back(glm::vec3(x + cellSize, 0, z + cellSize), color);
            vertices.emplace_back(glm::vec3(x, 0, z + cellSize), color);

            const unsigned int start = (row * cols + col) * 4;

            indices.push_back(start + 0);
            indices.push_back(start + 1);
            indices.push_back(start + 2);

            indices.push_back(start + 0);
            indices.push_back(start + 2);
            indices.push_back(start + 3);
        }
    }

    const auto terrain = new Mesh(name);
    terrain->InitFromData(vertices, indices);
    return terrain;
}

Mesh* object3D::CreateParallelepiped(const std::string& name,
                                     const glm::vec3 dimensions,
                                     glm::vec3 color) {
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    const glm::vec3 half = dimensions * 0.5f;

    vertices.emplace_back(glm::vec3(-half.x, -half.y, -half.z), color);
    vertices.emplace_back(glm::vec3(half.x, -half.y, -half.z), color);
    vertices.emplace_back(glm::vec3(half.x, -half.y, half.z), color);
    vertices.emplace_back(glm::vec3(-half.x, -half.y, half.z), color);

    vertices.emplace_back(glm::vec3(-half.x, half.y, -half.z), color);
    vertices.emplace_back(glm::vec3(half.x, half.y, -half.z), color);
    vertices.emplace_back(glm::vec3(half.x, half.y, half.z), color);
    vertices.emplace_back(glm::vec3(-half.x, half.y, half.z), color);

    unsigned int idx[] = {
        0, 1, 2, 2, 3, 0,
        4, 5, 6, 6, 7, 4,
        0, 1, 5, 5, 4, 0,
        2, 3, 7, 7, 6, 2,
        0, 3, 7, 7, 4, 0,
        1, 2, 6, 6, 5, 1
    };

    indices.insert(indices.end(), idx, idx + 36);

    const auto mesh = new Mesh(name);
    mesh->InitFromData(vertices, indices);
    return mesh;
}

Mesh* object3D::CreateCylinder(const std::string& name,
                               const float radius,
                               const float height,
                               glm::vec3 color) {
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    constexpr int SEG = 32;
    const float step = 2.0f * kPi / SEG;

    vertices.emplace_back(glm::vec3(0, 0, 0), color);
    vertices.emplace_back(glm::vec3(0, height, 0), color);

    for (int index = 0; index <= SEG; index++) {
        const float a = static_cast<float>(index) * step;
        const float x = radius * cos(a);
        const float z = radius * sin(a);

        vertices.emplace_back(glm::vec3(x, 0, z), color);
        vertices.emplace_back(glm::vec3(x, height, z), color);
    }

    for (int index = 0; index < SEG; index++) {
        constexpr int ringStart = 2;
        constexpr int centerTop = 1;
        constexpr int centerBottom = 0;

        const int b0 = ringStart + index * 2;
        const int t0 = b0 + 1;
        const int b1 = ringStart + (index + 1) * 2;
        const int t1 = b1 + 1;

        indices.push_back(b0);
        indices.push_back(b1);
        indices.push_back(t1);

        indices.push_back(b0);
        indices.push_back(t1);
        indices.push_back(t0);

        indices.push_back(centerBottom);
        indices.push_back(b1);
        indices.push_back(b0);

        indices.push_back(centerTop);
        indices.push_back(t0);
        indices.push_back(t1);
    }

    const auto mesh = new Mesh(name);
    mesh->InitFromData(vertices, indices);
    return mesh;
}

Mesh* object3D::CreatePyramid(const std::string& name,
                              const float base,
                              const float height,
                              glm::vec3 color) {
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;
    const float h = base * 0.5f;

    vertices.emplace_back(glm::vec3(-h, 0, -h), color);
    vertices.emplace_back(glm::vec3(h, 0, -h), color);
    vertices.emplace_back(glm::vec3(h, 0, h), color);
    vertices.emplace_back(glm::vec3(-h, 0, h), color);
    vertices.emplace_back(glm::vec3(0, height, 0), color);

    unsigned int idx[] = {
        0, 1, 2, 2, 3, 0,
        0, 1, 4,
        1, 2, 4,
        2, 3, 4,
        3, 0, 4
    };

    indices.insert(indices.end(), idx, idx + 18);

    const auto mesh = new Mesh(name);
    mesh->InitFromData(vertices, indices);
    return mesh;
}

static Mesh* RailStraight(const std::string& name,
                          const float tileSize,
                          const glm::vec3& baseColor,
                          const glm::vec3& railColor,
                          const bool addSleepers) {
    const float trackW = tileSize * 0.5f;
    const float baseH = tileSize * 0.06f;
    const float railH = tileSize * 0.02f;

    const float railBarW = trackW * 0.12f;
    const float railLen = tileSize * 0.90f;

    const float zOff = trackW * 0.35f;
    const float yRail = baseH * 0.5f + railH * 0.5f + 0.001f;

    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    AppendBox(vertices, indices,
              glm::vec3(tileSize, baseH, trackW),
              baseColor,
              glm::vec3(0.0f, 0.0f, 0.0f));

    AppendBox(vertices, indices,
              glm::vec3(railLen, railH, railBarW),
              railColor,
              glm::vec3(0.0f, yRail, -zOff));

    AppendBox(vertices, indices,
              glm::vec3(railLen, railH, railBarW),
              railColor,
              glm::vec3(0.0f, yRail, +zOff));

    if (addSleepers) {
        const auto sleeperDim = glm::vec3(tileSize * 0.12f, railH * 1.2f, trackW * 0.85f);
        const float ySleeper = yRail + railH * 0.6f;
        const float xs[4] = {-tileSize * 0.30f, -tileSize * 0.10f, tileSize * 0.10f, tileSize * 0.30f};

        for (const float x : xs) {
            AppendBox(vertices, indices, sleeperDim, railColor, glm::vec3(x, ySleeper, 0.0f));
        }
    }

    auto* out = new Mesh(name);
    out->InitFromData(vertices, indices);
    return out;
}

static Mesh* RailCorner(const std::string& name,
                        const float tileSize,
                        const glm::vec3& baseColor,
                        const glm::vec3& railColor,
                        const bool addSleepers) {
    const float trackW = tileSize * 0.5f;
    const float baseH = tileSize * 0.06f;
    const float railH = tileSize * 0.02f;

    const float railBarW = trackW * 0.12f;
    const float armLen = tileSize * 0.50f;

    const float margin = tileSize * 0.05f;

    const float zOff = trackW * 0.35f;
    const float yRail = baseH * 0.5f + railH * 0.5f + 0.001f;

    const float innerStart = -trackW * 0.5f + margin;
    const float outerEnd = armLen - margin;

    const float armRailLen = outerEnd - innerStart;
    const float armRailCenter = (outerEnd + innerStart) * 0.5f;

    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    AppendBox(vertices, indices,
              glm::vec3(armLen, baseH, trackW),
              baseColor,
              glm::vec3(armLen * 0.5f, 0.0f, 0.0f));
    AppendBox(vertices, indices,
              glm::vec3(trackW, baseH, armLen),
              baseColor,
              glm::vec3(0.0f, 0.0f, armLen * 0.5f));
    AppendBox(vertices, indices,
              glm::vec3(trackW, baseH, trackW),
              baseColor,
              glm::vec3(0.0f, 0.0f, 0.0f));

    AppendBox(vertices, indices,
              glm::vec3(armRailLen, railH, railBarW),
              railColor,
              glm::vec3(armRailCenter, yRail, -zOff));
    AppendBox(vertices, indices,
              glm::vec3(armRailLen, railH, railBarW),
              railColor,
              glm::vec3(armRailCenter, yRail, +zOff));

    AppendBox(vertices, indices,
              glm::vec3(railBarW, railH, armRailLen),
              railColor,
              glm::vec3(-zOff, yRail, armRailCenter));
    AppendBox(vertices, indices,
              glm::vec3(railBarW, railH, armRailLen),
              railColor,
              glm::vec3(+zOff, yRail, armRailCenter));

	if (addSleepers) { // adauga traversele
        const float ySleeper = yRail + railH * 0.6f;

        const auto sleeperXDim = glm::vec3(tileSize * 0.10f, railH * 1.2f, trackW * 0.85f);
        const auto sleeperZDim = glm::vec3(trackW * 0.85f, railH * 1.2f, tileSize * 0.10f);

        AppendBox(vertices, indices, sleeperXDim, railColor, glm::vec3(armLen * 0.62f, ySleeper, 0.0f));
        AppendBox(vertices, indices, sleeperZDim, railColor, glm::vec3(0.0f, ySleeper, armLen * 0.62f));
    }

    auto* out = new Mesh(name);
    out->InitFromData(vertices, indices);
    return out;
}

Mesh* object3D::CreateRailNormalStraight(const std::string& name, const float tileSize) {
    const auto baseColor = glm::vec3(0.08f, 0.08f, 0.10f);
    const auto railColor = glm::vec3(0.70f, 0.70f, 0.70f);
    return RailStraight(name, tileSize, baseColor, railColor, false);
}

Mesh* object3D::CreateRailNormalCorner(const std::string& name, const float tileSize) {
    const auto baseColor = glm::vec3(0.08f, 0.08f, 0.10f);
    const auto railColor = glm::vec3(0.70f, 0.70f, 0.70f);
    return RailCorner(name, tileSize, baseColor, railColor, false);
}

Mesh* object3D::CreateRailBridgeStraight(const std::string& name, const float tileSize) {
    const auto baseColor = glm::vec3(0.02f, 0.02f, 0.02f);
    const auto railColor = glm::vec3(0.95f, 0.95f, 0.95f);
    return RailStraight(name, tileSize, baseColor, railColor, false);
}

Mesh* object3D::CreateRailBridgeCorner(const std::string& name, float const tileSize) {
    const auto baseColor = glm::vec3(0.02f, 0.02f, 0.02f);
    const auto railColor = glm::vec3(0.95f, 0.95f, 0.95f);
    return RailCorner(name, tileSize, baseColor, railColor, false);
}

Mesh* object3D::CreateRailTunnelStraight(const std::string& name, const float tileSize) {
    const auto baseColor = glm::vec3(0.02f, 0.02f, 0.02f);
    const auto railColor = glm::vec3(0.90f, 0.90f, 0.90f);
    return RailStraight(name, tileSize, baseColor, railColor, true);
}

Mesh* object3D::CreateRailTunnelCorner(const std::string& name, const float tileSize) {
    const auto baseColor = glm::vec3(0.02f, 0.02f, 0.02f);
    const auto railColor = glm::vec3(0.90f, 0.90f, 0.90f);
    return RailCorner(name, tileSize, baseColor, railColor, true);
}
