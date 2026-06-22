#pragma once // Previne includerea multipla a headerului in acelasi TU

#include <string> // Folosit pentru std::string
#include <unordered_map> // Folosit pentru dictionarul de texturi
#include <vector> // Folosit pentru liste dinamice

#include "components/simple_scene.h" // Baza pentru scene simple din framework
#include "components/transform.h" // Utilitare de transformari

namespace m1
{
    class Tema3 : public gfxc::SimpleScene // Clasa scenei pentru Tema 3
    {
    public: // Sectiunea publica
        struct LightSource { // Structura pentru o sursa de lumina
            glm::vec3 position; // Pozitia luminii in spatiu
            glm::vec3 color; // Culoarea luminii
            glm::vec3 direction; // Directia pentru spotlight
            float cut_off; // Unghiul de taiere pentru spotlight
            int type; // 0: punctiforma, 1: spotlight
        }; // Sfarsit struct LightSource

        struct Firefly { // Structura pentru un licurici
            glm::vec3 center; // Centrul orbitei licuriciului
            float orbitRadius; // Raza orbitei
            float angularSpeed; // Viteza unghiulara
            float initialPhase; // Faza initiala a miscarii
            float verticalAmplitude; // Amplitudinea oscilatiei pe verticala
            glm::vec3 color; // Culoarea licuriciului
        }; // Sfarsit struct Firefly

        struct TreeInstance { // Structura pentru un copac instantiat
            glm::vec3 position; // Pozitia copacului in scena
            float scale; // Scara copacului
            float bendPhase; // Faza pentru animatia de vant
        }; // Sfarsit struct TreeInstance

        Tema3(); // Constructor
        ~Tema3(); // Destructor

        void Init() override; // Initializarea scenei

    private: // Sectiunea privata
        void FrameStart() override; // Apelata la inceputul fiecarui cadru
        void Update(float deltaTimeSeconds) override; // Actualizarea logica si randare
        void FrameEnd() override; // Apelata la sfarsitul fiecarui cadru

        void RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, // Randare mesh cu shader
            const glm::vec3& color, Texture2D* texture1 = NULL, Texture2D* texture2 = NULL, // Culoare si texturi
            int treePart = 0, float treeHeight = 1.0f, float bendStrength = 0.0f, float bendPhase = 0.0f, // Parametrii de animatie copac
            float emissiveStrength = 0.0f, float texcoordScale = 1.0f); // Emisie si tiling textura

        // Functii de input
        void OnInputUpdate(float deltaTime, int mods) override; // Input continuu
        void OnKeyPress(int key, int mods) override; // Input la apasare

        void RenderTower(glm::vec3 position); // Randarea turnului
        void RenderTree(const TreeInstance& tree); // Randarea unui copac

    protected: // Sectiunea protejata
        std::unordered_map<std::string, Texture2D*> mapTextures; // Dictionar de texturi
        std::vector<LightSource> lightSources; // Lista de lumini active
        std::vector<Firefly> fireflies; // Lista de licurici
        std::vector<TreeInstance> trees; // Lista de copaci
        Texture2D* cookieTexture = nullptr; // Textura pentru light cookie

        glm::vec3 fogColor; // Culoarea cetii
        float fogMaxDist; // Distanta maxima a cetii (folosita ca referinta)
        float fogDensity; // Densitatea cetii pentru modelul exponential
    };
}