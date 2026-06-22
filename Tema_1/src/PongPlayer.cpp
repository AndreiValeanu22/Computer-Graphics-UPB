#include "../include/gl_includes.h"
#include "../include/PongPlayer.h"
//#include <GL/gl.h>
// PONG
PongPlayer::PongPlayer(float x_, float y_, float w, float h, float s)
    : x(x_), y(y_), width(w), height(h), speed(s) {
    // initializeaza paleta cu datele de pozitie si viteza
}

void PongPlayer::MoveUp(float dt) {
    y += speed * dt; // misca in sus
    if (y + height > 600) y = 600 - height; // ramane in ecran
}

void PongPlayer::MoveDown(float dt) {
    y -= speed * dt; // misca in jos
    if (y < 0) y = 0; // ramane in ecran
}

void PongPlayer::TriggerAnimation() { animating = true; animTime = 0; } // porneste animatia la ricoseu

void PongPlayer::Animate(float dt) {
    if (!animating) return;
    animTime += dt;
    if (animTime >= animDuration) animating = false; // opreste animatia
}

void PongPlayer::Render(float r, float g, float b) {
    glColor3f(r, g, b); // culoare variabila
    float currentHeight = height;
    if (animating) {
        float factor = 1.0f + (animScale - 1.0f) * (1.0f - animTime / animDuration);
        currentHeight = height * factor; // efect vizual de "bounce"
    }

    //glColor3f(1, 1, 1);
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + currentHeight);
    glVertex2f(x, y + currentHeight);
    glEnd();
}
