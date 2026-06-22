#include "../include/ParticleSystem.h"
#include <cstdlib>
#include <cmath>
// BREAKOUT

void ParticleSystem::Emit(float x, float y, int count, float r, float g, float b) {
    // creeaza count particule la pozitia x,y cu directie si viteza random
    for (int i = 0; i < count; ++i) {
        PS_Particle p;
        p.x = x;
        p.y = y;
        float angle = ((float)rand() / RAND_MAX) * 2.0f * 3.1415926f;
        float speed = 80.0f + (rand() % 100);
        p.vx = cosf(angle) * speed;
        p.vy = sinf(angle) * speed;
        p.lifetime = 0.4f + ((float)rand() / RAND_MAX) * 0.6f;
        p.r = r; p.g = g; p.b = b;
        particles.push_back(p);
    }
}

void ParticleSystem::Update(float dt) {
    // actualizeaza pozitia, scade viata fiecarei particule, sterge cele disparute
    for (auto it = particles.begin(); it != particles.end();) {
        it->x += it->vx * dt;
        it->y += it->vy * dt;
        it->vy -= 200.0f * dt; // gravitatie
        it->lifetime -= dt;
        if (it->lifetime <= 0.0f) it = particles.erase(it);
        else ++it;
    }
}

void ParticleSystem::Render() {
    glPointSize(3.0f);
    glBegin(GL_POINTS);
    for (auto& p : particles) {
        glColor3f(p.r, p.g, p.b);
        glVertex2f(p.x, p.y);
    }
    glEnd();
}