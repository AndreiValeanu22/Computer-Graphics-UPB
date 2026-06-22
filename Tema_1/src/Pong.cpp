#include "../include/gl_includes.h"
#include "../include/Pong.h"
//#include <GL/gl.h>

#include <cmath>
#include <iostream>
// PONG

void Pong::Init() {
    player1 = PongPlayer(50, 250); // jucator stanga
    player2 = PongPlayer(730, 250); // jucator dreapta
    balls.clear(); // goleate mingi
    balls.push_back(Ball(400, 300, 10, 300)); // adauga o minge in centru
    score1 = score2 = 0; // scor initial
}

void Pong::Update(float dt) {
    GLFWwindow* window = glfwGetCurrentContext();
    // controale pentru fiecare jucator
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) player1.MoveUp(dt);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) player1.MoveDown(dt);
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) player2.MoveUp(dt);
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) player2.MoveDown(dt);

    player1.Animate(dt); // animatii ricoseu la fiecare paleta (optional)
    player2.Animate(dt);

    for (auto& ball : balls) {
        ball.Update(dt);

        // Coliziune cu paleta 1
        if (ball.x - ball.radius < player1.x + player1.width &&
            ball.x - ball.radius > player1.x &&
            ball.y > player1.y && ball.y < player1.y + player1.height)
        {
            float relativeY = (ball.y - (player1.y + player1.height / 2));
            float norm = relativeY / (player1.height / 2);
            float angle = norm * (3.14159f / 4);
            ball.Bounce(angle, 1);

            ball.x = player1.x + player1.width + ball.radius + 1; // fortare back in play

            player1.TriggerAnimation();
        }

        // Coliziune cu paleta 2
        if (ball.x + ball.radius > player2.x &&
            ball.x + ball.radius < player2.x + player2.width &&
            ball.y > player2.y && ball.y < player2.y + player2.height)
        {
            float relativeY = (ball.y - (player2.y + player2.height / 2));
            float norm = relativeY / (player2.height / 2);
            float angle = norm * (3.14159f / 4);
            ball.Bounce(angle, -1);
            
			ball.x = player2.x - ball.radius - 1; // fortare back in play

            player2.TriggerAnimation();
        }

        // Scor
        if (ball.x < -ball.radius) { score2++; ball.Reset(1); }
        if (ball.x > 800 + ball.radius) { score1++; ball.Reset(-1); }
    }
}

void Pong::Render() {
    // Linie punctata
    glColor3f(1, 1, 1);
    for (int i = 0; i < 600; i += 20) {
        glBegin(GL_LINES);
        glVertex2f(400, i);
        glVertex2f(400, i + 10);
        glEnd();
    }

    // Delimitari sus/jos
    glBegin(GL_LINES);
    glVertex2f(0, 0); glVertex2f(800, 0);
    glVertex2f(0, 600); glVertex2f(800, 600);
    glEnd();

    // In Pong.cpp, functia void Pong::Render()
    glColor3f(1.0f, 1.0f, 0.2f); // galben pentru player1 (stanga)
    player1.Render(1.0f, 1.0f, 0.2f);

    glColor3f(1.0f, 0.0f, 0.2f); // rosu pentru player2 (dreapta)
    player2.Render(1.0f, 0.0f, 0.2f);



    for (auto& ball : balls) ball.Render();

    // scor in terminal
    std::cout << "\rScore: " << score1 << " - " << score2 << std::flush;
}
