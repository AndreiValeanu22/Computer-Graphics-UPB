#include "../include/gl_includes.h"
#include "../include/Enemy.h"
//#include <GL/gl.h>
#include <cstdlib>
#include <cmath>

// CHICKEN INVADERS
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

Enemy::Enemy(float x_, float y_, int health_)
    : x(x_), y(y_), width(40), height(30), speedX(50), speedY(20),
    health(health_), alive(true), shootTimer(0),
    initialX(x_), amplitude(50), frequency(2.0f), elapsedTime(0) {
    // initializeaza inamicul cu parametri de baza si miscare/lovituri
}

void Enemy::Update(float dt) {
    elapsedTime += dt;

    // Oscilatie sinusoidala pe X
    x = initialX + amplitude * sin(frequency * elapsedTime);

    // Coborare graduala
    y -= speedY * dt;
}

void Enemy::Shoot(std::vector<Projectile>& enemyProjectiles, float dt) {
    if (!alive) return;
    shootTimer -= dt;           // scade timerul cu delta time
    if (shootTimer <= 0) {
        enemyProjectiles.emplace_back(x + width / 2, y, -200, false);
        shootTimer = 2.0f + rand() % 3; // delay random la urmatoarea impuscare
    }
}


void Enemy::Render() {
    if (!alive) return;

    float cx = x + width / 2;
    float cy = y + height / 2;
    int segments = 20;
    // desenam corp, capul, creasta, ciocul si aripile din triunghiuri si fani
    // Corp oval
    glColor3f(1.0f, 1.0f, 0.98f);
    glBegin(GL_TRIANGLE_FAN);
    for (int i = 0; i <= 22; i++) {
        float theta = i * 2 * M_PI / 22;
        glVertex2f(cx + (width / 2) * cos(theta), cy + (height * 0.6) * sin(theta));
    }
    glEnd();

    // Cap mic
    glColor3f(1.0f, 0.85f, 0.7f);
    float head_cx = cx + width * 0.3f;
    float head_cy = cy - height * 0.10f;
    glBegin(GL_TRIANGLE_FAN);
    for (int i = 0; i <= 16; i++) {
        float theta = i * 2 * M_PI / 16;
        glVertex2f(head_cx + 10 * cos(theta), head_cy + 10 * sin(theta));
    }
    glEnd();

    // Creasta roșie
    glColor3f(0.9f, 0.0f, 0.0f);
    glBegin(GL_TRIANGLES);
    glVertex2f(head_cx, head_cy + 10);
    glVertex2f(head_cx + 4, head_cy + 16);
    glVertex2f(head_cx - 4, head_cy + 16);
    glEnd();

    // Cioc portocaliu
    glColor3f(1.0f, 0.6f, 0.0f);
    glBegin(GL_TRIANGLES);
    glVertex2f(head_cx + 10, head_cy);
    glVertex2f(head_cx + 16, head_cy + 2);
    glVertex2f(head_cx + 10, head_cy + 6);
    glEnd();

    // Aripi stânga-dreapta
    glColor3f(0.8f, 0.82f, 0.95f);
    glBegin(GL_TRIANGLE_FAN);
    for (int i = 0; i <= 10; i++) {
        float theta = M_PI / 4 + i * M_PI / 3 / 10;
        glVertex2f(cx - width / 3 + 10 * cos(theta), cy + 5 + 10 * sin(theta));
    }
    glEnd();
    glBegin(GL_TRIANGLE_FAN);
    for (int i = 0; i <= 10; i++) {
        float theta = 3 * M_PI / 4 - i * M_PI / 3 / 10;
        glVertex2f(cx + width / 3 + 10 * cos(theta), cy + 5 + 10 * sin(theta));
    }
    glEnd();

}
