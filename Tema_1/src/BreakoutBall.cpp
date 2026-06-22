#include "../include/BreakoutBall.h"
#include <cmath>
// BREAKOUT
BreakoutBall::BreakoutBall(float x_, float y_, float vx_, float vy_)
    : x(x_), y(y_), vx(vx_), vy(vy_) {
    // constructor: initializeaza pozitie si viteza
}

void BreakoutBall::Update(float dt) {
    x += vx * dt; // actualizeaza pozitia pe x
    y += vy * dt; // actualizeaza pozitia pe y
    // coliziune cu peretii lateral si sus
    if (x - radius < 0) { x = radius; vx = -vx; } // ricoseu la stanga
    if (x + radius > 800) { x = 800 - radius; vx = -vx; } // ricoseu la dreapta
    if (y + radius > 600) { y = 600 - radius; vy = -vy; } // ricoseu la tavan
    // josul e gestionat in Breakout.cpp pentru viata pierduta
}

void BreakoutBall::Render() {
    glColor3f(1.0f, 1.0f, 1.0f); // bila alba
    glBegin(GL_TRIANGLE_FAN); // desenare cerc simplu
    for (int i = 0; i <= 24; ++i) { // imparit in 24 de segmente
        float theta = (float)i / 24.0f * 2.0f * 3.1415926f;
        glVertex2f(x + radius * cosf(theta), y + radius * sinf(theta));
    }
    glEnd();
}
