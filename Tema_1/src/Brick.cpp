//#include <GL/gl.h>
#include "../include/gl_includes.h"
#include "../include/Brick.h"
// BREAKOUT

Brick::Brick(float x_, float y_, int hits_)
    : x(x_), y(y_), hits(hits_), alive(true), destroying(false), destroyTimer(0.0f) {
    // constructor: initializeaza pozitie si rezistenta
}

void Brick::Hit() {
    if (!alive || destroying) return; // daca e deja mort sau in animatie de disparitie, iesim
    hits--; // scade rezistenta la fiecare lovitura
    if (hits <= 0) {
        destroying = true; // pornim animatia
        destroyTimer = 0.0f; // reset timer
    }
}

void Brick::UpdateAnimation(float dt) {
    if (!destroying) return;
    destroyTimer += dt; // creste timpul de animatie de disparitie
    if (destroyTimer >= 0.4f) {
        alive = false; // la final brick-ul dispare complet
        destroying = false;
    }
}

void Brick::Render() {
    if (!alive && !destroying) return; // nu desenam daca e complet disparut

    // culoare in functie de hits (culoarea afisata este culoarea *inainte* de decrement)
    // culoare in functie de rezistenta ramas
    float r = 1.0f, g = 1.0f, b = 1.0f;
    int col = hits;
    if (!destroying) col = hits;
    // map hits -> color (1..5+, cap la 7)
    int mapHits = hits;
    if (mapHits <= 0) mapHits = 1;
    switch (mapHits) {
    case 1: r = 0.0f; g = 0.6f; b = 1.0f; break; // albastru
    case 2: r = 0.0f; g = 1.0f; b = 0.0f; break; // verde
    case 3: r = 1.0f; g = 0.0f; b = 0.0f; break; // rosu
    case 4: r = 1.0f; g = 1.0f; b = 0.0f; break; // galben
    case 5: r = 0.6f; g = 0.3f; b = 0.0f; break; // maro
    case 6: r = 0.5f; g = 0.5f; b = 0.5f; break; // gri
    default: r = 0.6f; g = 0.0f; b = 0.6f; break; // mov
    }

    float cx = x + width * 0.5f;
    float cy = y + height * 0.5f;
    float scale = 1.0f;
    if (destroying) { // animatie: micsoram brick-ul la disparitie
        scale = 1.0f - (destroyTimer / 0.4f); // de la 1 -> 0
        if (scale < 0.0f) scale = 0.0f;
    }

    float w = width * scale;
    float h = height * scale;

    glColor3f(r, g, b);
    glBegin(GL_QUADS);
    glVertex2f(cx - w / 2, cy - h / 2);
    glVertex2f(cx + w / 2, cy - h / 2);
    glVertex2f(cx + w / 2, cy + h / 2);
    glVertex2f(cx - w / 2, cy + h / 2);
    glEnd();
}