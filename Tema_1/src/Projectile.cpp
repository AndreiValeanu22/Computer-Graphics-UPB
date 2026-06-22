#include "../include/gl_includes.h"
//#include <GL/gl.h>
#include <cmath>
#include "../include/Projectile.h"

// CHICKEN INVADERS
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

Projectile::Projectile(float x, float y, float speedY, bool fromPlayer)
    : x(x), y(y), speedY(speedY), fromPlayer(fromPlayer), radius(7), alive(true) {
    // constructor: initializeaza proiectilul cu parametri
}

void Projectile::Update(float dt) {
    y += speedY * dt; // deplaseaza proiectilul
    if (y < 0 || y > 600) alive = false; // cand iese din ecran, dispare
}

void Projectile::Render() {
    if (!alive) return;

    if (!fromPlayer) { // proiectil inamic -> ou alb
        glColor3f(1.0f, 1.0f, 1.0f); // alb

        glBegin(GL_TRIANGLE_FAN);
        int segments = 40;
        float verticalScale = 1.5f; // alungire pe axa Y ca sa semene a ou
        for (int i = 0; i <= segments; i++) {
            float theta = i / (float)segments * 2.0f * M_PI;
            // forma ovala
            float rx = radius * cos(theta);
            float ry = radius * sin(theta) * verticalScale;
            glVertex2f(x + rx, y + ry);
        }
        glEnd();

        // mic luciu in varf (bonus vizual)
        glColor3f(1.0f, 1.0f, 0.9f);
        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(x, y + radius * 1.2f);
        for (int i = 0; i <= 10; i++) {
            float theta = i / 10.0f * M_PI;
            float rx = radius * 0.4f * cos(theta);
            float ry = radius * 0.4f * sin(theta);
            glVertex2f(x + rx, y + ry + radius);
        }
        glEnd();
    }
    /*else { proiectil jucator -> patrat verde
        glColor3f(0.0f, 1.0f, 0.0f);
        glBegin(GL_QUADS);
        glVertex2f(x - 2, y);
        glVertex2f(x + 2, y);
        glVertex2f(x + 2, y + 10);
        glVertex2f(x - 2, y + 10);
        glEnd();
    */
    else { // proiectil jucator -> oval verde
        glColor3f(0.0f, 1.0f, 0.35f); // verde deschis (ca in demo)

        glBegin(GL_TRIANGLE_FAN);
        int segments = 40;
        float verticalScale = 1.2f; // putin mai mic decat la ou
        for (int i = 0; i <= segments; i++) {
            float theta = i / (float)segments * 2.0f * M_PI;
            float rx = radius * cos(theta);
            float ry = radius * sin(theta) * verticalScale;
            glVertex2f(x + rx, y + ry);
        }
        glEnd();

        // highlight/luciu (bonus vizual optional)
        glColor3f(0.8f, 1.0f, 0.8f);
        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(x, y + radius * 0.8f);
        for (int i = 0; i <= 10; i++) {
            float theta = i / 10.0f * M_PI;
            float rx = radius * 0.2f * cos(theta);
            float ry = radius * 0.2f * sin(theta);
            glVertex2f(x + rx, y + ry + radius * 0.4f);
        }
        glEnd();
    }
}
