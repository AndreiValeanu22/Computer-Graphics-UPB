#include "include/gl_includes.h"
#include <cstdio>

#include "include/EditorNava.h"
#include "include/Breakout.h"
#include "include/ChickenInvaders.h"
#include "include/Pong.h"

// selector scene
enum class Scene { EDITOR = 0, BREAKOUT = 1, CHICKEN = 2, PONG = 3 }; // scena curenta, ctrl+tasta

// mic helper de text (GLUT bitmap)
static void DrawText(float x, float y, const char* s) { // scrie text cu font bitmap GLUT la coordonate date
    glColor3f(1, 1, 1); // alb
    glRasterPos2f(x, y); // pozitia unde se incepe
    for (const char* p = s; *p; ++p) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *p); // scrie fiecare caracter
}

int main(int argc, char** argv) {
    // GLUT doar pentru text bitmap
    glutInit(&argc, argv); // initializeaza GLUT

    // --- GLFW ---
    if (!glfwInit()) return -1; // initializeaza fereastra si input
    GLFWwindow* window = glfwCreateWindow(800, 600, "Tema1 - Launcher (F1..F4)", nullptr, nullptr); // creaza fereastra principala
    if (!window) { glfwTerminate(); return -1; } // failed, se inchide
    glfwMakeContextCurrent(window); // setam contextul curent pentru rendering

    // --- GLEW (dupa ce avem context) ---
    if (glewInit() != GLEW_OK) { glfwTerminate(); return -1; } // initializeaza GLEW

    // --- 2D ortho ---
    glViewport(0, 0, 800, 600); // viewport fix pentru rezolutie standard
    glMatrixMode(GL_PROJECTION); glLoadIdentity();
    glOrtho(0, 800, 0, 600, -1, 1); // setam proiectia ortogonala pentru 2D
    glMatrixMode(GL_MODELVIEW); glLoadIdentity();
    glDisable(GL_DEPTH_TEST); // se dezactiveaza testul de adancime pentru 2D

    // --- Instante jocuri ---
    EditorNava editor;          // are Update(dt_fix, window)
    Breakout breakout;          // are Init/Update/Render
    breakout.Init();
    ChickenInvaders chicken;    // are Init/Update/Render
    chicken.Init();
    Pong pong;                  // are Init/Update/Render
    pong.Init();

    Scene scene = Scene::EDITOR; // scena de start — editorul

    float last = (float)glfwGetTime(); // timestamp initial pentru masurare timpi
    while (!glfwWindowShouldClose(window)) {
        float now = (float)glfwGetTime();
        float dt = now - last;
        last = now;

        // input global
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GLFW_TRUE); // inchide fereastra la ESC

        // comutare scene — ctrl rapid cu tastele F1–F4 sau 1–4
        if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) scene = Scene::EDITOR;
        if (glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) scene = Scene::BREAKOUT;
        if (glfwGetKey(window, GLFW_KEY_F3) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) scene = Scene::CHICKEN;
        if (glfwGetKey(window, GLFW_KEY_F4) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) scene = Scene::PONG;

        glClearColor(0, 0, 0, 1); // fundal negru
        glClear(GL_COLOR_BUFFER_BIT); // sterge frame

        // transmite nava editata la ChickenInvaders cand e start in editor
        if (editor.startPressed && scene == Scene::EDITOR) {
            // Transmite nava catre ChickenInvaders!
            chicken.SetNava(editor.GetVehicul()); // trimite componentele navei in joc
        }


        switch (scene) {
        case Scene::EDITOR:
            editor.Update(0.016f, window); // editor-ul foloseste delta timp constant
            editor.Render();
            DrawText(12, 12, "F2: Breakout | F3: Chicken | F4: Pong | ESC: Quit");
            break;

        case Scene::BREAKOUT:
            breakout.Update(dt);
            breakout.Render();
            DrawText(12, 12, "F1: Editor | F3: Chicken | F4: Pong | SPACE: Launch | ESC: Quit");
            break;

        case Scene::CHICKEN:
            chicken.Update(dt);
            chicken.Render();
            DrawText(12, 12, "F1: Editor | F2: Breakout | F4: Pong | ESC: Quit");
            break;

        case Scene::PONG:
            pong.Update(dt);
            pong.Render();
            DrawText(12, 12, "F1: Editor | F2: Breakout | F3: Chicken | ESC: Quit");
            break;
        }

        glfwSwapBuffers(window); // afiseaza pe ecran
        glfwPollEvents(); // preia toate inputurile
    }

    glfwTerminate(); // finalizeaza, curata totul
    return 0;
}

/*
* INAINTE
* g++ main_pong.cpp src/Pong.cpp src/PongPlayer.cpp src/Ball.cpp -Iinclude -lglfw -lGL -lGLEW -lGLU -lglut -o Pong 
./Pong
* g++ main_editor.cpp src/EditorNava.cpp src/Componenta.cpp -Iinclude -lglfw -lGL -lGLEW -lGLU -lglut -o EditorNava 
./EditorNava
* g++ main_chicken.cpp src/ChickenInvaders.cpp src/CIPlayer.cpp src/Enemy.cpp src/Projectile.cpp -Iinclude -lglfw -lGL -lGLEW -lGLU -lglut -o ChickenInvaders 
./ChickenInvaders
* g++ main_breakout.cpp src/Breakout.cpp src/BreakoutBall.cpp src/Brick.cpp src/PaddleBreakout.cpp src/ParticleSystem.cpp -Iinclude -lglfw -lGL -lGLEW -lGLU -lglut -o Breakout 
./Breakout
INAINTE

ACTUAL

In folderul Tema1: g++ -std=c++17 MainLauncher.cpp src/*.cpp -Iinclude -lglfw -lGL -lGLEW -lGLU -lglut -ldl -o Tema1Launcher
ACTUAL
*/