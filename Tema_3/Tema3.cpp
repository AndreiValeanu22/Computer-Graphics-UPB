#include "lab_m1/Tema3/Tema3.h" // Include headerul clasei

#include <vector> // Vectori STL
#include <string> // Stringuri STL
#include <iostream> // I/O pentru debug

using namespace std; // Folosim spatiul de nume std
using namespace m1; // Folosim spatiul de nume m1

Tema3::Tema3() { // Constructor
    fogColor = glm::vec3(0.5f, 0.5f, 0.6f); // Culoarea cetii (gri-albastrui)
    fogMaxDist = 100.0f; // Distanta maxima pentru ceata
    fogDensity = 0.03f; // Densitatea pentru ceata exponentiala
}

Tema3::~Tema3() {} // Destructor gol

void Tema3::Init() { // Initializarea scenei
    const string sourceDir = PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema3", "textures"); // Directorul texturilor

    // Incarcare texturi cu extensiile tale specifice
    { // Bloc de incarcare texturi
        mapTextures["bark"] = new Texture2D(); // Creeaza textura pentru scoarta
        mapTextures["bark"]->Load2D(PATH_JOIN(sourceDir, "bark.jpg").c_str(), GL_REPEAT); // Incarca textura scoarta

        mapTextures["leaves"] = new Texture2D(); // Creeaza textura pentru frunze
        mapTextures["leaves"]->Load2D(PATH_JOIN(sourceDir, "leaves.jpg").c_str(), GL_REPEAT); // Incarca textura frunze

        mapTextures["wood_planks"] = new Texture2D(); // Creeaza textura pentru scanduri
        mapTextures["wood_planks"]->Load2D(PATH_JOIN(sourceDir, "wood_planks.png").c_str(), GL_REPEAT); // Incarca textura scanduri

        // Pastram si restul daca mai ai nevoie de ele
        mapTextures["grass"] = new Texture2D(); // Creeaza textura pentru iarba
        mapTextures["grass"]->Load2D(PATH_JOIN(sourceDir, "grass.jpeg").c_str(), GL_REPEAT); // Incarca textura iarba

        mapTextures["wood"] = new Texture2D(); // Creeaza textura pentru lemn simplu
        mapTextures["wood"]->Load2D(PATH_JOIN(sourceDir, "wood.jpg").c_str(), GL_REPEAT); // Incarca textura lemn

        // Folosim o textura existenta drept light cookie
        cookieTexture = mapTextures["leaves"]; // Seteaza textura de cookie
    }

    // Incarcare primitive
    { // Bloc de incarcare meshuri
        Mesh* mesh = new Mesh("box"); // Creeaza mesh pentru cub
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj"); // Incarca cubul
        meshes[mesh->GetMeshID()] = mesh; // Salveaza meshul

        mesh = new Mesh("sphere"); // Creeaza mesh pentru sfera
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj"); // Incarca sfera
        meshes[mesh->GetMeshID()] = mesh; // Salveaza meshul

        mesh = new Mesh("cylinder"); // Creeaza mesh pentru cilindru
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema3", "textures"), "cylinder.obj"); // Incarca cilindrul
        meshes[mesh->GetMeshID()] = mesh; // Salveaza meshul
    }

    // Configurare licurici (exemplu: 5 licurici)
    for (int i = 0; i < 5; i++) { // Bucla pentru licurici
        Firefly f; // Instanta licurici
        f.center = glm::vec3(0, 2, 0); // Centrul orbitei
        f.orbitRadius = 3.0f + i; // Raza orbitei
        f.angularSpeed = 0.5f + i * 0.2f; // Viteza unghiulara
        f.initialPhase = i * 1.5f; // Faza initiala
        f.verticalAmplitude = 0.5f; // Amplitudine verticala
        float g = 0.7f + 0.3f * (rand() / (float)RAND_MAX); // Variatie componenta verde
        float r = 0.6f + 0.3f * (rand() / (float)RAND_MAX); // Variatie componenta rosie
        f.color = glm::vec3(r, 1.0f, g); // Culoare galben-verde
        fireflies.push_back(f); // Adauga licuriciul in lista
    }

    // Creare Shader
    Shader* shader = new Shader("ForestShader"); // Creeaza shaderul scenei
    shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema3", "Shaders", "VertexShader.glsl"), GL_VERTEX_SHADER); // Ataseaza vertex shader
    shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema3", "Shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER); // Ataseaza fragment shader
    shader->CreateAndLink(); // Compileaza si leaga shaderul
    shaders[shader->GetName()] = shader; // Salveaza shaderul in map

    // Cream componentele turnului
    /*{ // Bloc dezactivat pentru stalpi din cilindru
        Mesh* mesh = new Mesh("pillar"); // Creeaza mesh pentru stalp
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "cylinder.obj"); // Incarca cilindrul
        meshes[mesh->GetMeshID()] = mesh; // Salveaza meshul
    }*/ // Sfarsit bloc dezactivat

    { // Incarcare platforma
        Mesh* mesh = new Mesh("platform"); // Creeaza mesh pentru platforma
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj"); // Incarca box pentru platforma
        meshes[mesh->GetMeshID()] = mesh; // Salveaza meshul
    } // Sfarsit incarcare platforma

    /*{ // Bloc dezactivat pentru acoperis din primitives
        Mesh* mesh = new Mesh("roof"); // Creeaza mesh pentru acoperis
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "cone.obj"); // Incarca con
        meshes[mesh->GetMeshID()] = mesh; // Salveaza meshul
    }*/ // Sfarsit bloc dezactivat
    { // Incarcare acoperis
        Mesh* mesh = new Mesh("roof"); // Creeaza mesh pentru acoperis
        // cone.obj este in folderul Tema3/textures (langa texturi), nu in RESOURCE_PATH::MODELS
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema3", "textures"), "cone.obj"); // Incarca conul
        meshes[mesh->GetMeshID()] = mesh; // Salveaza meshul
    } // Sfarsit incarcare acoperis

    // Configurare copaci (pozitii statice pentru inceput)
    { // Bloc pentru copaci
        trees.clear(); // Curata lista de copaci
        trees.push_back({ glm::vec3(-10, 0, -8), 1.2f, 0.2f }); // Copac 1
        trees.push_back({ glm::vec3(-12, 0, 6), 1.0f, 1.1f }); // Copac 2
        trees.push_back({ glm::vec3(8, 0, -10), 1.4f, 2.0f }); // Copac 3
        trees.push_back({ glm::vec3(12, 0, 7), 1.1f, 2.6f }); // Copac 4
        trees.push_back({ glm::vec3(0, 0, -14), 1.3f, 3.4f }); // Copac 5
        trees.push_back({ glm::vec3(-6, 0, 12), 0.9f, 4.2f }); // Copac 6
        trees.push_back({ glm::vec3(6, 0, 12), 1.0f, 5.0f }); // Copac 7
        trees.push_back({ glm::vec3(14, 0, 0), 1.2f, 5.8f }); // Copac 8
    } // Sfarsit configurare copaci
    // REPOZITIONARE CAMERA:
    // Punem camera la 10 unitati in spate si 5 unitati in sus, privind spre centrul scenei (0,0,0)
    GetSceneCamera()->SetPosition(glm::vec3(0, 5, 15)); // Pozitioneaza camera
    GetSceneCamera()->SetRotation(glm::vec3(RADIANS(-15), 0, 0)); // Roteste camera usor in jos
    GetSceneCamera()->Update(); // Actualizeaza camera
} // Sfarsit Init

void Tema3::FrameStart() // Inceput de cadru
{ // Deschidere FrameStart
    // Seteaza culoarea de fundal (R, G, B, A) - un albastru deschis
    glClearColor(0.5f, 0.7f, 1.0f, 1); // Culoarea background = ceata
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Curata bufferul de culoare si adancime

    glm::ivec2 resolution = window->GetResolution(); // Citeste rezolutia ferestrei
    glViewport(0, 0, resolution.x, resolution.y); // Seteaza viewportul
} // Sfarsit FrameStart

void Tema3::Update(float deltaTimeSeconds) { // Update pe cadru
    float time = (float)Engine::GetElapsedTime(); // Timpul global al motorului
    lightSources.clear(); // Goleste lista de lumini

    // 1. Logica Licurici (Punctele negre devin sfere luminoase)
    for (auto& f : fireflies) { // Itereaza licuricii
        float x = f.center.x + f.orbitRadius * cos(f.angularSpeed * time + f.initialPhase); // Pozitia X pe orbita
        float z = f.center.z + f.orbitRadius * sin(f.angularSpeed * time + f.initialPhase); // Pozitia Z pe orbita
        float y = f.center.y + f.verticalAmplitude * sin(time * 2.0f + f.initialPhase); // Oscilatie pe Y
        glm::vec3 pos = glm::vec3(x, y, z); // Pozitia finala

        // Randare corp licurici - folosim culoarea lor ca emisie
        glm::mat4 modelMatrix = glm::mat4(1); // Matrice identitate
        modelMatrix = glm::translate(modelMatrix, pos); // Translate la pozitia licuriciului
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f)); // Scale mic pentru sfera
        RenderSimpleMesh(meshes["sphere"], shaders["ForestShader"], modelMatrix, f.color, NULL, cookieTexture, // Deseneaza sfera
            0, 1.0f, 0.0f, 0.0f, 2.5f, 1.0f); // Seteaza emisie puternica

        LightSource ls; // Lumina punctiforma
        ls.position = pos; // Pozitia luminii
        ls.color = f.color; // Culoarea luminii
        ls.type = 0; // Tip punctiform
        lightSources.push_back(ls); // Adauga lumina in lista
    } // Sfarsit licurici

    // 2. Spotlight-uri din varful turnului
    { // Primul spotlight
        LightSource spot; // Creeaza spotlight
        spot.position = glm::vec3(0, 6.5f, 0); // Pozitionat sub acoperis
        spot.direction = glm::normalize(glm::vec3(sin(time), -1, cos(time))); // Directie rotativa
        spot.color = glm::vec3(1, 1, 1); // Culoare alba
        spot.cut_off = 30.0f; // Unghiul spotului
        spot.type = 1; // Tip spotlight
        lightSources.push_back(spot); // Adauga spotlight
    } // Sfarsit primul spotlight
    { // Al doilea spotlight
        LightSource spot; // Creeaza spotlight
        spot.position = glm::vec3(0, 6.5f, 0); // Pozitie comuna
        spot.direction = glm::normalize(glm::vec3(sin(time + 3.14f), -1, cos(time + 3.14f))); // Directie opusa
        spot.color = glm::vec3(0.8f, 0.9f, 1.0f); // Culoare usor albastruie
        spot.cut_off = 28.0f; // Unghi mai strans
        spot.type = 1; // Tip spotlight
        lightSources.push_back(spot); // Adauga spotlight
    } // Sfarsit al doilea spotlight

    // 3. Randare Teren (Iarba revine aici)
    { // Bloc teren
        glm::mat4 modelMatrix = glm::mat4(1); // Matrice identitate
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -0.05f, 0)); // Pozitionare usor sub 0
        modelMatrix = glm::scale(modelMatrix, glm::vec3(50, 0.1f, 50)); // Scara mare pentru plan

        // Daca grass.png arata rau, foloseste NULL si o culoare verde inchis
        glm::vec3 grassColor = glm::vec3(0.1f, 0.5f, 0.1f); // Culoare fallback
        RenderSimpleMesh(meshes["box"], shaders["ForestShader"], modelMatrix, grassColor, mapTextures["grass"], cookieTexture, // Deseneaza teren
            0, 1.0f, 0.0f, 0.0f, 0.0f, 20.0f); // Textura repetata de 20 ori
    } // Sfarsit teren

    // 4. Randare Copaci
    for (const auto& t : trees) { // Itereaza copacii
        RenderTree(t); // Deseneaza copacul
    } // Sfarsit copaci

    // 5. Randare Turn (Asamblarea din primitive)
    RenderTower(glm::vec3(0, 0, 0)); // Deseneaza turnul
} // Sfarsit Update

void Tema3::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, // Functie de randare
    const glm::vec3& color, Texture2D* texture1, Texture2D* texture2, // Parametri de culoare si texturi
    int treePart, float treeHeight, float bendStrength, float bendPhase, // Parametri pentru copaci
    float emissiveStrength, float texcoordScale) { // Parametri pentru emisie si UV
    if (!mesh || !shader || !shader->GetProgramID()) return; // Verificare validitate

    glUseProgram(shader->program); // Foloseste programul de shader

    // Uniforme standard
    glUniformMatrix4fv(glGetUniformLocation(shader->program, "Model"), 1, GL_FALSE, glm::value_ptr(modelMatrix)); // Model
    glUniformMatrix4fv(glGetUniformLocation(shader->program, "View"), 1, GL_FALSE, glm::value_ptr(GetSceneCamera()->GetViewMatrix())); // View
    glUniformMatrix4fv(glGetUniformLocation(shader->program, "Projection"), 1, GL_FALSE, glm::value_ptr(GetSceneCamera()->GetProjectionMatrix())); // Projection

    glUniform3fv(glGetUniformLocation(shader->program, "eye_position"), 1, glm::value_ptr(GetSceneCamera()->m_transform->GetWorldPosition())); // Pozitia camerei
    glUniform3fv(glGetUniformLocation(shader->program, "object_color"), 1, glm::value_ptr(color)); // Culoare obiect
    glUniform3fv(glGetUniformLocation(shader->program, "fog_color"), 1, glm::value_ptr(fogColor)); // Culoare ceata
    glUniform1f(glGetUniformLocation(shader->program, "fog_max_dist"), fogMaxDist); // Distanta maxima ceata
    glUniform1f(glGetUniformLocation(shader->program, "fog_density"), fogDensity); // Densitate ceata
    glUniform1f(glGetUniformLocation(shader->program, "time"), (float)Engine::GetElapsedTime()); // Timp global
    glUniform1i(glGetUniformLocation(shader->program, "tree_part"), treePart); // Tip parte copac
    glUniform1f(glGetUniformLocation(shader->program, "tree_height"), treeHeight); // Inaltime copac
    glUniform1f(glGetUniformLocation(shader->program, "bend_strength"), bendStrength); // Amplitudine vant
    glUniform1f(glGetUniformLocation(shader->program, "bend_phase"), bendPhase); // Faza vant
    glUniform1f(glGetUniformLocation(shader->program, "emissive_strength"), emissiveStrength); // Intensitate emisie
    glUniform1f(glGetUniformLocation(shader->program, "texcoord_scale"), texcoordScale); // Multiplicare UV

    // Trimitere array de lumini
    for (int i = 0; i < lightSources.size(); i++) { // Itereaza luminile
        string base = "lights[" + to_string(i) + "]."; // Baza nume uniforma
        glUniform3fv(glGetUniformLocation(shader->program, (base + "position").c_str()), 1, glm::value_ptr(lightSources[i].position)); // Pozitie lumina
        glUniform3fv(glGetUniformLocation(shader->program, (base + "color").c_str()), 1, glm::value_ptr(lightSources[i].color)); // Culoare lumina
        glUniform3fv(glGetUniformLocation(shader->program, (base + "direction").c_str()), 1, glm::value_ptr(lightSources[i].direction)); // Directie lumina
        glUniform1f(glGetUniformLocation(shader->program, (base + "cut_off").c_str()), RADIANS(lightSources[i].cut_off)); // Unghiul spotului
        glUniform1i(glGetUniformLocation(shader->program, (base + "type").c_str()), lightSources[i].type); // Tipul luminii
    } // Sfarsit iterare lumini
    glUniform1i(glGetUniformLocation(shader->program, "num_lights"), (int)lightSources.size()); // Trimite numarul de lumini

    // Texturi
    if (texture1) { // Daca textura principala exista
        glActiveTexture(GL_TEXTURE0); // Activeaza unitatea 0
        glBindTexture(GL_TEXTURE_2D, texture1->GetTextureID()); // Bindeaza textura
        glUniform1i(glGetUniformLocation(shader->program, "texture_1"), 0); // Seteaza samplerul 0
    } // Sfarsit textura principala
    if (texture2) { // Daca textura cookie exista
        glActiveTexture(GL_TEXTURE1); // Activeaza unitatea 1
        glBindTexture(GL_TEXTURE_2D, texture2->GetTextureID()); // Bindeaza textura cookie
        glUniform1i(glGetUniformLocation(shader->program, "texture_cookie"), 1); // Seteaza samplerul 1
    } // Sfarsit textura cookie

    glBindVertexArray(mesh->GetBuffers()->m_VAO); // Leaga VAO-ul
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0); // Deseneaza meshul
} // Sfarsit RenderSimpleMesh

void Tema3::FrameEnd() {} // Sfarsit de cadru
void Tema3::OnInputUpdate(float deltaTime, int mods) {} // Input continuu nefolosit
void Tema3::OnKeyPress(int key, int mods) {} // Input la apasare nefolosit

void Tema3::RenderTower(glm::vec3 position) { // Randarea turnului
    float pillarHeight = 8.0f; // Inaltimea stalpilor de jos
    float platformSize = 3.5f; // Latimea platformei
    float cabinHeight = 1.2f; // Inaltimea balustradei

    // 1. STALPII DE JOS (Cei 4 piloni lungi)
    glm::vec3 offsets[] = { // Offseturi pentru colturi
        glm::vec3(-1.3f, 0, -1.3f), glm::vec3(1.3f, 0, -1.3f), // Spate
        glm::vec3(-1.3f, 0, 1.3f),  glm::vec3(1.3f, 0, 1.3f) // Fata
    }; // Sfarsit offseturi
    for (int i = 0; i < 4; i++) { // Itereaza stalpii
        glm::mat4 modelMatrix = glm::mat4(1); // Matrice identitate
        modelMatrix = glm::translate(modelMatrix, position + offsets[i] + glm::vec3(0, pillarHeight / 2, 0)); // Pozitioneaza stalpul
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.15f, pillarHeight, 0.15f)); // Scale pentru stalp
        RenderSimpleMesh(meshes["box"], shaders["ForestShader"], modelMatrix, glm::vec3(1), mapTextures["bark"], cookieTexture); // Deseneaza stalpul
    } // Sfarsit stalpi jos

    // 2. PODEAUA (Baza platformei)
    { // Bloc podea
        glm::mat4 modelMatrix = glm::mat4(1); // Matrice identitate
        modelMatrix = glm::translate(modelMatrix, position + glm::vec3(0, pillarHeight, 0)); // Pozitionare pe varful stalpilor
        modelMatrix = glm::scale(modelMatrix, glm::vec3(platformSize, 0.2f, platformSize)); // Scale pentru podea
        RenderSimpleMesh(meshes["box"], shaders["ForestShader"], modelMatrix, glm::vec3(1), mapTextures["wood_planks"], cookieTexture); // Deseneaza podeaua
    } // Sfarsit podea

    // 3. BALUSTRADA / BORDURA (Peretii scurti ai cabinei)
    // Aceasta sectiune creeaza marginile groase de lemn de pe platforma
    float wallThickness = 0.25f; // Grosimea peretelui
    glm::vec3 wallOffsets[] = { // Offseturi pentru pereti
        glm::vec3(0, 0, 1.65f),  // Fata
        glm::vec3(0, 0, -1.65f), // Spate
        glm::vec3(1.65f, 0, 0),  // Dreapta
        glm::vec3(-1.65f, 0, 0)  // Stanga
    }; // Sfarsit offseturi pereti

    for (int i = 0; i < 4; i++) { // Itereaza peretii
        glm::mat4 modelMatrix = glm::mat4(1); // Matrice identitate
        modelMatrix = glm::translate(modelMatrix, position + glm::vec3(0, pillarHeight + cabinHeight / 2, 0) + wallOffsets[i]); // Pozitioneaza peretele

        if (i < 2) { // Pereti fata-spate
            modelMatrix = glm::scale(modelMatrix, glm::vec3(platformSize, cabinHeight, wallThickness)); // Scale pe lungime
        }
        else { // Pereti laterali
            modelMatrix = glm::scale(modelMatrix, glm::vec3(wallThickness, cabinHeight, platformSize)); // Scale pe latime
        }
        RenderSimpleMesh(meshes["box"], shaders["ForestShader"], modelMatrix, glm::vec3(1), mapTextures["bark"], cookieTexture); // Deseneaza peretele
    } // Sfarsit pereti

    // 4. STALPII SUPERIORI (Cei 4 stalpi subtiri care urca din colturi)
    float upperPillarHeight = 2.5f; // Inaltime stalpi superiori
    for (int i = 0; i < 4; i++) { // Itereaza stalpii superiori
        glm::mat4 modelMatrix = glm::mat4(1); // Matrice identitate
        modelMatrix = glm::translate(modelMatrix, position + offsets[i] + glm::vec3(0, pillarHeight + upperPillarHeight / 2, 0)); // Pozitioneaza stalpul
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.12f, upperPillarHeight, 0.12f)); // Scale stalp superior
        RenderSimpleMesh(meshes["box"], shaders["ForestShader"], modelMatrix, glm::vec3(1), mapTextures["bark"], cookieTexture); // Deseneaza stalpul
    } // Sfarsit stalpi superiori

    // 5. ACOPERISUL (Conul din varf)
    { // Bloc acoperis
        glm::mat4 modelMatrix = glm::mat4(1); // Matrice identitate
        // 8.0 (piloni) + 2.5 (stalpi superiori) = 10.5 inaltime totala
        modelMatrix = glm::translate(modelMatrix, position + glm::vec3(0, 10.5f, 0)); // Pozitioneaza acoperisul
        modelMatrix = glm::scale(modelMatrix, glm::vec3(4.5f, 2.0f, 4.5f)); // Scale pentru acoperis

        // In imaginea ta, acoperisul pare sa fie din scanduri, deci wood_planks e ok
        RenderSimpleMesh(meshes["roof"], shaders["ForestShader"], modelMatrix, glm::vec3(1), mapTextures["wood_planks"], cookieTexture); // Deseneaza acoperisul
    } // Sfarsit acoperis
} // Sfarsit RenderTower

void Tema3::RenderTree(const TreeInstance& tree) { // Randarea unui copac
    // Parametri comuni
    const float trunkHeight = 3.5f * tree.scale; // Inaltime trunchi
    const float trunkRadius = 0.35f * tree.scale; // Raza trunchi
    const float foliageBaseY = trunkHeight * 0.7f; // Baza frunzis

    // 1. TRUNCHI (folosim cilindru)
    { // Bloc trunchi
        glm::mat4 modelMatrix = glm::mat4(1); // Matrice identitate
        modelMatrix = glm::translate(modelMatrix, tree.position + glm::vec3(0, trunkHeight * 0.5f, 0)); // Pozitionare trunchi
        modelMatrix = glm::scale(modelMatrix, glm::vec3(trunkRadius, trunkHeight, trunkRadius)); // Scale trunchi
        RenderSimpleMesh(meshes["cylinder"], shaders["ForestShader"], modelMatrix, // Deseneaza trunchiul
            glm::vec3(1), mapTextures["bark"], cookieTexture, 1, trunkHeight, 0.0f, tree.bendPhase); // Setare parte copac
    } // Sfarsit trunchi

    // 2. FRUNZIS (6 etaje de cutii care se micsoreaza)
    float leafHeight = 0.8f * tree.scale; // Inaltime frunzis
    float leafWidth = 2.4f * tree.scale; // Latime frunzis
    for (int i = 0; i < 6; i++) { // Itereaza nivelurile
        float t = i / 5.0f; // Normalizare nivel
        float levelScale = 1.0f - t * 0.6f; // Scadere treptata a dimensiunii
        glm::mat4 modelMatrix = glm::mat4(1); // Matrice identitate
        modelMatrix = glm::translate(modelMatrix, // Pozitionare frunzis
            tree.position + glm::vec3(0, foliageBaseY + i * (leafHeight * 0.6f), 0)); // Inaltime nivel
        modelMatrix = glm::scale(modelMatrix, glm::vec3(leafWidth * levelScale, leafHeight, leafWidth * levelScale)); // Scale frunzis
        RenderSimpleMesh(meshes["box"], shaders["ForestShader"], modelMatrix, // Deseneaza frunzisul
            glm::vec3(1), mapTextures["leaves"], cookieTexture, 2, trunkHeight, 0.25f, tree.bendPhase + t); // Animatie vant
    } // Sfarsit frunzis
} // Sfarsit RenderTree