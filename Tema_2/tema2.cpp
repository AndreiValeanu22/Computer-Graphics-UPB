#include "tema2.h"
#include "object3D.h"

using namespace m1;

const float kPi = std::acos(-1.0f); // Linia asta defineste constanta Pi

Tema2::Tema2() = default;

Tema2::~Tema2() = default;

void Tema2::Init() {
	railGrid.assign(ROWS, std::vector<RailSegment>(COLS)); // Linia asta initializeaza railGrid cu dimensiunile ROWS x COLS
    toolCursorRow = 0;
    toolCursorCol = 0;

    train.row = 5;
    train.col = 8;
    train.dir = Dir::E;
    train.prevDir = Dir::E;
    train.alpha = 0.0f;
    train.tiles = 0;

    LoadShader("Terrain");
    LoadShader("GhostTrail");

    GenerateTerrain();
    GenerateRails();
    GenerateTrainParts();
    GenerateStations();

    poseHistory.clear();
    PushPoseSample();
}

void Tema2::GenerateTerrain() {
	terrain.resize(ROWS, std::vector<TerrainType>(COLS, TerrainType::Field)); // Linia asta initializeaza matricea terrain cu tipul Field

    for (unsigned int row = 8; row <= 19; row++) {
        for (unsigned int col = 8; col <= 9; col++) {
            terrain[row][col] = TerrainType::River;
        }
    }

    for (unsigned int row = 8; row <= 9; row++) {
        for (unsigned int col = 8; col <= 16; col++) {
            terrain[row][col] = TerrainType::River;
        }
    }

    for (unsigned int row = 4; row <= 8; row++) {
        for (unsigned int col = 15; col <= 16; col++) {
            terrain[row][col] = TerrainType::River;
        }
    }

    for (unsigned int row = 4; row <= 5; row++) {
        for (unsigned int col = 16; col <= 19; col++) {
            terrain[row][col] = TerrainType::River;
        }
    }

    for (unsigned int row = 1; row <= 6; row++) {
        for (unsigned int col = 1; col <= 12; col++) {
            terrain[row][col] = TerrainType::Mountain;
        }
    }

    std::vector<std::vector<glm::vec3>> colors;
	colors.resize(ROWS, std::vector<glm::vec3>(COLS)); // Linia asta initializeaza matricea colors cu dimensiunile ROWS x COLS

    const auto FIELD_COLOR = glm::vec3(0.75f, 0.95f, 0.75f);
    const auto RIVER_COLOR = glm::vec3(0.45f, 0.85f, 1.0f);
    const auto MOUNTAIN_COLOR = glm::vec3(0.75f, 0.65f, 0.45f);

    for (unsigned int row = 0; row < ROWS; row++) {
        for (unsigned int col = 0; col < COLS; col++) {
            switch (terrain[row][col]) {
            case TerrainType::Field:
                colors[row][col] = FIELD_COLOR;
                break;
            case TerrainType::River:
                colors[row][col] = RIVER_COLOR;
                break;
            case TerrainType::Mountain:
                colors[row][col] = MOUNTAIN_COLOR;
                break;
            }
        }
    }

    meshes["terrain"] = object3D::CreateTerrain("terrain", ROWS, COLS, CELL, colors);
}

void Tema2::GenerateRails() {
	// Liniile astea creeaza mesh-urile pentru diferitele tipuri de sine
    meshes["rail_normal_straight"] = object3D::CreateRailNormalStraight("rail_normal_straight", CELL);
    meshes["rail_normal_corner"] = object3D::CreateRailNormalCorner("rail_normal_corner", CELL);
    meshes["rail_bridge_straight"] = object3D::CreateRailBridgeStraight("rail_bridge_straight", CELL);
    meshes["rail_bridge_corner"] = object3D::CreateRailBridgeCorner("rail_bridge_corner", CELL);
    meshes["rail_tunnel_straight"] = object3D::CreateRailTunnelStraight("rail_tunnel_straight", CELL);
    meshes["rail_tunnel_corner"] = object3D::CreateRailTunnelCorner("rail_tunnel_corner", CELL);

    constexpr int minRow = 5;
    constexpr int maxRow = 12;
    constexpr int minCol = 4;
    constexpr int maxCol = 14;

    for (int col = minCol + 1; col < maxCol; col++) {
        PlaceStraight(minRow, col, Dir::W, Dir::E);
        PlaceStraight(maxRow, col, Dir::W, Dir::E);
    }
    for (int row = minRow + 1; row < maxRow; row++) {
        PlaceStraight(row, minCol, Dir::N, Dir::S);
        PlaceStraight(row, maxCol, Dir::N, Dir::S);
    }

    PlaceCorner(minRow, minCol, Dir::E, Dir::S);
    PlaceCorner(minRow, maxCol, Dir::S, Dir::W);
    PlaceCorner(maxRow, maxCol, Dir::W, Dir::N);
    PlaceCorner(maxRow, minCol, Dir::N, Dir::E);
}

void Tema2::GenerateTrainParts() {
    meshes["wagon_body"] = object3D::CreateParallelepiped("wagon_body", WAGON_BODY_SIZE,
                                                          glm::vec3(0.4f, 0.9f, 0.4f));

    meshes["locomotive_cabin"] = object3D::CreateParallelepiped("locomotive_cabin", CABIN_SIZE,
                                                                glm::vec3(0.4f, 0.9f, 0.4f));

    meshes["engine"] = object3D::CreateCylinder("engine", ENGINE_RADIUS, ENGINE_LENGTH,
                                                glm::vec3(0.1f, 0.2f, 0.8f));

    meshes["wheel"] = object3D::CreateCylinder("wheel", WHEEL_RADIUS, WHEEL_WIDTH,
                                               glm::vec3(0.9f, 0.1f, 0.1f));
}

void Tema2::GenerateStations() {
    meshes["station_box"] = object3D::CreateParallelepiped("station_box",
                                                           glm::vec3(0.9f, 0.5f, 0.6f),
                                                           glm::vec3(0.9f, 0.3f, 0.3f));

    meshes["station_cylinder"] = object3D::CreateCylinder("station_cylinder",
                                                          0.35f, 0.6f,
                                                          glm::vec3(0.3f, 0.9f, 0.3f));

    meshes["station_pyramid"] = object3D::CreatePyramid("station_pyramid",
                                                        0.8f, 0.7f,
                                                        glm::vec3(0.3f, 0.3f, 0.9f));

    stations.clear();
    stations.push_back({
        6, 7, Dir::N, "station_box"
    });
    stations.push_back({
        12, 10, Dir::E, "station_cylinder"
    });
    stations.push_back({
        9, 4, Dir::S, "station_pyramid"
    });
}

bool Tema2::InBounds(const int row, const int col) {
    return row >= 0 && row < ROWS && col >= 0 && col < COLS;
}

Tema2::RailStyle Tema2::StyleFromTerrain(const TerrainType type) {
    switch (type) {
    case TerrainType::Field: return RailStyle::Normal;
    case TerrainType::River: return RailStyle::Bridge;
    case TerrainType::Mountain: return RailStyle::Tunnel;
    }

    return RailStyle::Normal;
}

std::string Tema2::RailMeshKey(const RailStyle style, const RailShape shape) {
    switch (style) {
    case RailStyle::Normal:
        return (shape == RailShape::Straight) ? "rail_normal_straight" : "rail_normal_corner";
    case RailStyle::Bridge:
        return (shape == RailShape::Straight) ? "rail_bridge_straight" : "rail_bridge_corner";
    case RailStyle::Tunnel:
        return (shape == RailShape::Straight) ? "rail_tunnel_straight" : "rail_tunnel_corner";
    }

    return "rail_normal_straight";
}

int Tema2::CountConnections(const RailSegment& tile) {
    int count = 0;

    for (const bool connection : tile.connections) {
        if (connection) {
            count++;
        }
    }

    return count;
}

void Tema2::AddConnection(const int row, const int col, const Dir dir) {
    if (!InBounds(row, col)) {
        return;
    }

    auto& tile = railGrid[row][col];
    tile.occupied = true;
    tile.connections[static_cast<int>(dir)] = true;
    tile.style = StyleFromTerrain(terrain[row][col]);
}

void Tema2::PlaceStraight(const int row, const int col, const Dir a, const Dir b) {
	const auto& tile = railGrid[row][col]; // Linia asta obtine referinta catre celula curenta din railGrid
	const int existing = CountConnections(tile); // Linia asta numara conexiunile existente in celula curenta

    if (existing >= 2) {
        return;
    }

    AddConnection(row, col, a);
    AddConnection(row, col, b);
}

void Tema2::PlaceCorner(const int row, int const col, const Dir a, const Dir b) {
    AddConnection(row, col, a);
    AddConnection(row, col, b);
}

void Tema2::RenderTileRails(const int row, const int col, const RailSegment& tile) {
    constexpr float y = 0.03f;
	const glm::vec3 basePos(static_cast<float>(col) + 0.5f, y, static_cast<float>(row) + 0.5f); // Linia asta calculeaza pozitia de baza a celulei

    auto render = [&](const RailShape shape, const float rotation) {
        glm::mat4 model(1);
        model = glm::translate(model, basePos);
        model = glm::rotate(model, rotation, glm::vec3(0, 1, 0));
        RenderMesh(meshes[RailMeshKey(tile.style, shape)], shaders["VertexColor"], model);
    };

    const bool N = tile.connections[Dir::N];
    const bool E = tile.connections[Dir::E];
    const bool S = tile.connections[Dir::S];
    const bool W = tile.connections[Dir::W];

    if (N && S) {
        render(RailShape::Straight, kPi * 0.5f);
    }

    if (E && W) {
        render(RailShape::Straight, 0.0f);
    }

    if (E && S) {
        render(RailShape::Corner, 0.0f);
    }

    if (S && W) {
        render(RailShape::Corner, kPi * 1.5f);
    }

    if (W && N) {
        render(RailShape::Corner, kPi);
    }

    if (N && E) {
        render(RailShape::Corner, kPi * 0.5f);
    }
}

void Tema2::DrawWheel(const glm::vec3& localPosition, const float rotation, const glm::mat4& parent) {
    glm::mat4 model = parent;
    model = glm::translate(model, localPosition);
    model = glm::rotate(model, RADIANS(90), glm::vec3(0, 0, 1));
    model = glm::rotate(model, RADIANS(90), glm::vec3(1, 0, 0));
    model = glm::rotate(model, rotation, glm::vec3(1, 0, 0));
    RenderMesh(meshes["wheel"], shaders["VertexColor"], model);
}

void Tema2::DrawWagon(const glm::mat4& parent, const float wheelRotation) {
    glm::mat4 body = parent;
    body = glm::translate(body, glm::vec3(0.0f, 0.3f, 0.0f));
    RenderMesh(meshes["wagon_body"], shaders["VertexColor"], body);

    constexpr float x = 0.25f;
    const float y = WHEEL_RADIUS;
    constexpr float z = TRACK_HALF_WIDTH;

    DrawWheel({-x, y, -z - WHEEL_WIDTH * 0.6f}, wheelRotation, parent);
    DrawWheel({-x, y, z - WHEEL_WIDTH * 0.4f}, wheelRotation, parent);
    DrawWheel({x, y, -z - WHEEL_WIDTH * 0.6f}, wheelRotation, parent);
    DrawWheel({x, y, z - WHEEL_WIDTH * 0.4f}, wheelRotation, parent);
}

void Tema2::DrawLocomotive(const glm::mat4& parent, const float wheelRotation) {
    glm::mat4 cabin = parent;
    cabin = glm::translate(cabin, glm::vec3(-0.2f, 0.375f, 0.0f));
    RenderMesh(meshes["locomotive_cabin"], shaders["VertexColor"], cabin);

    glm::mat4 engine = parent;
    engine = glm::translate(engine, glm::vec3(0.5f, 0.3f, 0.0f));
    engine = glm::rotate(engine, RADIANS(90), glm::vec3(0, 0, 1));
    RenderMesh(meshes["engine"], shaders["VertexColor"], engine);

    for (int index = -2; index <= 2; index++) {
        float x = static_cast<float>(index) * 0.18f;
        const float y = WHEEL_RADIUS;
        constexpr float z = TRACK_HALF_WIDTH;

        DrawWheel({x, y, -z - WHEEL_WIDTH * 0.6f}, wheelRotation, parent);
        DrawWheel({x, y, z - WHEEL_WIDTH * 0.4f}, wheelRotation, parent);
    }
}

void Tema2::RenderTrain(const float time) {
    const float wheelRotation = time * 4.0f;
    const float headDistance = (static_cast<float>(train.tiles) + train.alpha) * CELL;

	// Liniile astea deseneaza locomotiva si vagoanele trenului
    {
        const PoseSample poseSample = PoseAtDistance(headDistance);

        glm::mat4 locomotive(1);
        locomotive = glm::translate(locomotive, poseSample.pos);
        locomotive = glm::rotate(locomotive, poseSample.rotY, glm::vec3(0, 1, 0));
        DrawLocomotive(locomotive, wheelRotation);
    }

    for (int index = 1; index <= wagonCount; index++) {
        const float distance = headDistance - static_cast<float>(index) * wagonSpacing;
        PoseSample poseSample = PoseAtDistance(distance);

        glm::mat4 wagon(1);
        wagon = glm::translate(wagon, poseSample.pos);
        wagon = glm::rotate(wagon, poseSample.rotY, glm::vec3(0, 1, 0));
        DrawWagon(wagon, wheelRotation);
    }
}

glm::mat4 Tema2::TrainOnTile(const int row, int const col, const float rotationY) {
    glm::mat4 m(1);
    m = glm::translate(m, glm::vec3(static_cast<float>(col) + 0.5f, 0.05f, static_cast<float>(row) + 0.5f));
    m = glm::rotate(m, rotationY, glm::vec3(0, 1, 0));
    return m;
}

glm::ivec2 Tema2::DirDelta(const Dir dir) {
    switch (dir) {
    case Dir::N: return {0, -1};
    case Dir::S: return {0, 1};
    case Dir::E: return {1, 0};
    case Dir::W: return {-1, 0};
    }

    return {0, 0};
}

float Tema2::DirRotationY(const Dir dir) {
    switch (dir) {
    case Dir::E: return 0.0f;
    case Dir::W: return kPi;
    case Dir::N: return kPi * 0.5f;
    case Dir::S: return kPi * 1.5f;
    }

    return 0.0f;
}

float Tema2::LerpAngle(const float a, const float b, const float t) {
    float diff = std::fmod(b - a + kPi, 2.0f * kPi);

    if (diff < 0) {
        diff += 2.0f * kPi;
    }

    diff -= kPi;
    return a + diff * t;
}

bool Tema2::NextDir(const int row, const int col, const Dir in, Dir& out) const {
    const auto& tile = railGrid[row][col];

    if (!tile.occupied || !tile.connections[in]) {
        return false;
    }

    for (int index = 0; index < 4; index++) {
        const auto dir = static_cast<Dir>(index);

        if (tile.connections[dir] && dir != in) {
            out = dir;
            return true;
        }
    }

    return false;
}

glm::vec3 Tema2::TrainWorldPos() const {
	const glm::vec3 center(static_cast<float>(train.col) + 0.5f, 0.05f, static_cast<float>(train.row) + 0.5f); // Linia asta calculeaza pozitia centrala a trenului pe celula curenta

    const glm::ivec2 d0 = DirDelta(train.prevDir);
    const glm::ivec2 d1 = DirDelta(train.dir);

    const glm::vec3 entry = center - glm::vec3(d0.x, 0, d0.y) * 0.5f;
    const glm::vec3 exit = center + glm::vec3(d1.x, 0, d1.y) * 0.5f;

    if (train.prevDir == train.dir) {
        return glm::mix(entry, exit, train.alpha);
    }

    if (train.alpha < 0.5f) {
        return glm::mix(entry, center, train.alpha * 2.0f);
    }

    return glm::mix(center, exit, (train.alpha - 0.5f) * 2.0f);
}

float Tema2::TrainRotationY() const {
    const float a = DirRotationY(train.prevDir);
    const float b = DirRotationY(train.dir);

    if (train.prevDir == train.dir) {
        return b;
    }

    float u = train.alpha;
    u = u * u * (3.0f - 2.0f * u);
    return LerpAngle(a, b, u);
}

void Tema2::PushPoseSample() {
    const float headDistance = (static_cast<float>(train.tiles) + train.alpha) * CELL;
    const float keep = wagonSpacing * (static_cast<float>(wagonCount) + 3); 

    PoseSample poseSample;
    poseSample.pos = TrainWorldPos();
    poseSample.rotY = TrainRotationY();
    poseSample.dist = headDistance;

    if (!poseHistory.empty() && headDistance <= poseHistory.back().dist + 1e-6f) {
        poseHistory.back() = poseSample;
    }

    else {
        poseHistory.push_back(poseSample);
    }

    while (poseHistory.size() >= 2 && (poseHistory.back().dist - poseHistory.front().dist) > keep) {
        poseHistory.pop_front();
    }
}

Tema2::PoseSample Tema2::PoseAtDistance(const float distance) const {
    if (poseHistory.empty()) {
        PoseSample poseSample;
        poseSample.pos = TrainWorldPos();
        poseSample.rotY = TrainRotationY();
        poseSample.dist = distance;
        return poseSample;
    }

    if (distance <= poseHistory.front().dist) {
        return poseHistory.front();
    }

    if (distance >= poseHistory.back().dist) {
        return poseHistory.back();
    }

    for (size_t index = 1; index < poseHistory.size(); index++) {
        const auto& b = poseHistory[index];

        if (b.dist >= distance) {
            const auto& a = poseHistory[index - 1];
            const float span = b.dist - a.dist;
            const float t = (span > 1e-6f) ? (distance - a.dist) / span : 0.0f;

            PoseSample out;
            out.dist = distance;
            out.pos = glm::mix(a.pos, b.pos, t);
            out.rotY = LerpAngle(a.rotY, b.rotY, t);
            return out;
        }
    }

    return poseHistory.back();
}

void Tema2::UpdateTrain(const float deltaTimeSeconds) {
    if (waitingAtStation) {
        stationStopTimer -= deltaTimeSeconds;

        if (stationStopTimer <= 0.0f) {
            waitingAtStation = false;
            train.speed = savedTrainSpeed;
        }

        return;
    }

    train.alpha += train.speed * deltaTimeSeconds;

    while (train.alpha >= 1.0f) {
        train.alpha -= 1.0f;

        const auto in = static_cast<Dir>((static_cast<int>(train.dir) + 2) % 4);
        const auto delta = DirDelta(train.dir);

        train.col += delta.x;
        train.row += delta.y;

        if (IsStationTile(train.row, train.col)) {
            waitingAtStation = true;
            stationStopTimer = 2.0f;
            savedTrainSpeed = train.speed;
            train.speed = 0.0f;
            return;
        }

        if (!InBounds(train.row, train.col) || !railGrid[train.row][train.col].occupied) {
            train.speed = 0.0f;
            return;
        }

        train.prevDir = train.dir;
        Dir next;

        if (!NextDir(train.row, train.col, in, next)) {
            train.speed = 0.0f;
            return;
        }

        train.dir = next;
        train.tiles++;
    }

    PushPoseSample();
}

glm::vec3 Tema2::OffsetFromRail(Dir side) {
    constexpr float d = CELL;

    switch (side) {
    case Dir::N: return {0, 0, -d};
    case Dir::S: return {0, 0, d};
    case Dir::E: return {d, 0, 0};
    case Dir::W: return {-d, 0, 0};
    }

    return {0, 0, 0};
}

bool Tema2::IsStationTile(int row, int col) const {
    for (const auto& station : stations) {
        if (station.row == row && station.col == col) {
            return true;
        }
    }
    return false;
}

void Tema2::RenderScene() {
    RenderMesh(meshes["terrain"], shaders["VertexColor"], glm::mat4(1));

    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLS; col++) {
            const auto& tile = railGrid[row][col];

            if (!tile.occupied) {
                continue;
            }

            RenderTileRails(row, col, tile);
        }
    }

    const RailStyle ghostStyle = StyleFromTerrain(terrain[toolCursorRow][toolCursorCol]);
    const std::string ghostMeshKey = RailMeshKey(ghostStyle, toolShape);

    glm::mat4 ghost(1.0f);
    ghost = glm::translate(ghost, glm::vec3(static_cast<float>(toolCursorCol) + 0.5f, 0.035f,
                                            static_cast<float>(toolCursorRow) + 0.5f));
    ghost = glm::rotate(ghost, toolRotationY, glm::vec3(0, 1, 0));

    Shader* shader = shaders["GhostTrail"];
    shader->Use();

	glUniform1f(glGetUniformLocation(shader->program, "uAlpha"), 0.5f); // uAlpha este o uniforma definita in shader-ul de fragment
    RenderMesh(meshes[ghostMeshKey], shader, ghost);
    glUniform1f(glGetUniformLocation(shader->program, "uAlpha"), 1.0f);

    for (const auto& station : stations) {
        glm::vec3 basePos(static_cast<float>(station.col) + 0.5f,
                          0.05f,
                          static_cast<float>(station.row) + 0.5f);

        glm::vec3 offset = OffsetFromRail(station.side);

        glm::mat4 model(1);
        model = glm::translate(model, basePos + offset);

        RenderMesh(meshes[station.meshKey], shaders["VertexColor"], model);
    }
}

void Tema2::FrameStart() {
    glClearColor(0.53f, 0.81f, 0.92f, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    const auto res = window->GetResolution();
    glViewport(0, 0, res.x, res.y);
}

void Tema2::Update(float deltaTimeSeconds) {
    UpdateTrain(deltaTimeSeconds);
    RenderScene();
    RenderTrain(deltaTimeSeconds);
}

void Tema2::FrameEnd() {
}

void Tema2::OnKeyPress(const int key, const int mods) {
    switch (key) {
    case GLFW_KEY_UP:
        toolCursorRow = std::min(ROWS - 1, toolCursorRow + 1);
        break;
    case GLFW_KEY_DOWN:
        toolCursorRow = std::max(0, toolCursorRow - 1);
        break;
    case GLFW_KEY_LEFT:
        toolCursorCol = std::min(COLS - 1, toolCursorCol + 1);
        break;
    case GLFW_KEY_RIGHT:
        toolCursorCol = std::max(0, toolCursorCol - 1);
        break;

    case GLFW_KEY_1:
        toolShape = RailShape::Straight;
        toolRotationY = 0.0f;
        break;

    case GLFW_KEY_2:
        toolShape = RailShape::Corner;
        toolRotationY = 0.0f;
        break;

    case GLFW_KEY_R:
        toolRotationY += kPi * 0.5f;

        if (toolRotationY >= kPi * 2.0f) {
            toolRotationY = 0.0f;
        }

        break;

    case GLFW_KEY_ENTER: {
        const int rotation = static_cast<int>(std::round(toolRotationY / (kPi * 0.5f))) & 3;

        if (toolShape == RailShape::Straight) {
            static constexpr Dir kStraightDirs[4][2] = {
                {Dir::W, Dir::E}, {Dir::N, Dir::S}, {Dir::W, Dir::E}, {Dir::N, Dir::S}
            };

            PlaceStraight(toolCursorRow, toolCursorCol, kStraightDirs[rotation][0], kStraightDirs[rotation][1]);
        }

        if (toolShape == RailShape::Corner) {
            static constexpr Dir kCornerDirs[4][2] = {
                {Dir::N, Dir::E}, {Dir::N, Dir::W}, {Dir::S, Dir::W}, {Dir::S, Dir::E}
            };

            PlaceCorner(toolCursorRow, toolCursorCol, kCornerDirs[rotation][0], kCornerDirs[rotation][1]);
        }

        break;
    }

    case GLFW_KEY_BACKSPACE:
        if (InBounds(toolCursorRow, toolCursorCol)) {
            railGrid[toolCursorRow][toolCursorCol] = RailSegment{};
        }

        break;
    default: ;
    }
}

void Tema2::LoadShader(const std::string& name) {
    const std::string shaderPath = PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2", "shaders");
    auto* shader = new Shader(name);
    shader->AddShader(PATH_JOIN(shaderPath, name + ".VS.glsl"), GL_VERTEX_SHADER);
    shader->AddShader(PATH_JOIN(shaderPath, name + ".FS.glsl"), GL_FRAGMENT_SHADER);
    shader->CreateAndLink();
    shaders[name] = shader;
}

/*
* Functionalitatiile de baza sunt facute. La avansate am facut meniul de plasare sine
* Din sageti misti, pe 1 selectezi sina dreapta, pe 2 colt de sina, cu enter plasezi si cu backspace stergi si pe r rotesti
* Am mai facut oprire in gara, dar nu merge tocmai ok. Se mai buguieste putin trenul.
* Am facut intersectii.
* Sina verde o mut si cu ea modific sinele, fie pun altele, fie le sterg.
*/