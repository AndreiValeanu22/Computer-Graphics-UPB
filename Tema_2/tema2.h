#pragma once

#include "components/camera.h"
#include "components/simple_scene.h"

#include <deque>
#include <map>
#include <vector>

namespace m1 {
    class Tema2 : public gfxc::SimpleScene {
    public:
        Tema2();
        ~Tema2() override;

        void Init() override;

    private:
        enum class TerrainType {
            Field,
            River,
            Mountain,
        };

        enum class RailStyle {
            Normal,
            Bridge,
            Tunnel,
        };

        enum class RailShape {
            Straight,
            Corner
        };

        enum Dir { N = 0, E = 1, S = 2, W = 3 };

        struct RailSegment {
            bool occupied = false;
            bool connections[4] = {false, false, false, false};
            RailStyle style = RailStyle::Normal;
            RailShape shape = RailShape::Straight;
            float rotationY = 0.0f;
        };

        struct TrainState {
            int row = 0;
            int col = 0;
            Dir dir = Dir::E;
            Dir prevDir = Dir::E;
            float alpha = 0.0f;
            float speed = 1.5f;
            long long tiles = 0;
        };

        struct PoseSample {
            glm::vec3 pos;
            float rotY = 0.0f;
            float dist = 0.0f;
        };

        struct Station {
            int row;
            int col;
            Dir side;
            std::string meshKey;
        };

        static constexpr int ROWS = 20;
        static constexpr int COLS = 20;
        static constexpr float CELL = 1.0f;
        static constexpr float TRACK_HALF_WIDTH = CELL * 0.5f * 0.35f;

        const glm::vec3 WAGON_BODY_SIZE = {0.9f, 0.35f, 0.5f};
        const glm::vec3 CABIN_SIZE = {0.55f, 0.5f, 0.5f};
        const float ENGINE_RADIUS = 0.2f;
        const float ENGINE_LENGTH = 0.8f;
        const float WHEEL_RADIUS = 0.10f;
        const float WHEEL_WIDTH = 0.08f;

        std::map<std::string, Mesh*> meshes;
        std::vector<std::vector<TerrainType>> terrain;
        std::vector<std::vector<RailSegment>> railGrid;

        int toolCursorRow = 0;
        int toolCursorCol = 0;

        RailShape toolShape = RailShape::Straight;
        float toolRotationY = 0.0f;

        TrainState train;
        int wagonCount = 3;

        std::deque<PoseSample> poseHistory;
        float wagonSpacing = CELL * 1.0f;

        std::vector<Station> stations;
        float stationStopTimer = 0.0f;
        bool waitingAtStation = false;
        float savedTrainSpeed = 0.0f;

        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;
        void OnKeyPress(int key, int mods) override;

        void RenderScene();
        void LoadShader(const std::string& name);

        void GenerateTerrain();
        void GenerateRails();
        void GenerateTrainParts();
        void GenerateStations();

		static bool InBounds(int row, int col); // verifica daca pozitia data este in limitele matricei
		static RailStyle StyleFromTerrain(TerrainType type); // obtine stilul caii ferate pe baza tipului de teren
		static std::string RailMeshKey(RailStyle style, RailShape shape); // obtine cheia mesh-ului pentru o combinatie data de stil si forma
		static int CountConnections(const RailSegment& tile); // numara conexiunile dintr-un segment de cale ferata

        void AddConnection(int row, int col, Dir dir);
        void PlaceStraight(int row, int col, Dir a, Dir b);
        void PlaceCorner(int row, int col, Dir a, Dir b);
        void RenderTileRails(int row, int col, const RailSegment& tile);

        void DrawWheel(const glm::vec3& localPosition, float rotation, const glm::mat4& parent);
        void DrawWagon(const glm::mat4& parent, float wheelRotation);
		void DrawLocomotive(const glm::mat4& parent, float wheelRotation); // deseneaza locomotiva in scena
		void RenderTrain(float time); // deseneaza trenul in scena

		static glm::mat4 TrainOnTile(int row, int col, float rotationY); // obtine matricea de transformare pentru tren pe o celula data
		static glm::ivec2 DirDelta(Dir dir); // obtine delta de pozitie pentru o directie data
		static float DirRotationY(Dir dir); // obtine rotatia pe axa Y pentru o directie data
		static float LerpAngle(float a, float b, float t); // interpoleaza unghiurile a si b cu factorul t

		bool NextDir(int row, int col, Dir in, Dir& out) const; // obtine urmatoarea directie a trenului pe baza conexiunilor din celula curenta
		glm::vec3 TrainWorldPos() const; // obtine pozitia mondiala a trenului
		float TrainRotationY() const; // obtine rotatia trenului pe axa Y
		void PushPoseSample(); // salveaza pozitia curenta a trenului in istoricul de pozitii
		PoseSample PoseAtDistance(float distance) const; // obtine pozitia si rotatia trenului la o anumita distanta parcursa
		void UpdateTrain(float deltaTimeSeconds); // actualizeaza pozitia trenului in functie de timpul scurs

		static glm::vec3 OffsetFromRail(Dir side); // calculeaza offset-ul pentru o statie in functie de directia caii ferate
		bool IsStationTile(int row, int col) const; // verifica daca o celula este o statie
    };
}
