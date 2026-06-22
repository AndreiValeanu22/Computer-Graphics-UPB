#include <cstdlib>
#include "../include/ParticleSystem_ChickenInvaders.h"
#include <cmath>

// CHICKEN INVADERS

void ParticleSystem_ChickenInvaders::Emit(float x, float y, int nr, float r, float g, float b) {
    // creaza nr de particule cu pozitie (x,y), culoare (r,g,b) si viteza random
    for (int i = 0; i < nr; ++i) {
        Particle p;
        p.x = x; p.y = y;
        float angle = ((float)rand() / RAND_MAX) * 2 * 3.14159f;
        float spd = 40 + rand() % 90;
        p.vx = cos(angle) * spd;
        p.vy = sin(angle) * spd;
        p.r = r; p.g = g; p.b = b; p.a = 1.0f; // a = alpha = opacitate
        p.lifetime = 0.4f + 0.5f * ((float)rand() / RAND_MAX);
        particles.push_back(p);
    }
}
void ParticleSystem_ChickenInvaders::Update(float dt) {
    // misca fiecare particula si scade viata; sterge cand expira
    for (auto it = particles.begin(); it != particles.end();) {
        it->x += it->vx * dt;
        it->y += it->vy * dt;
        it->lifetime -= dt;
        if (it->lifetime <= 0) it = particles.erase(it); else ++it;
    }
}
void ParticleSystem_ChickenInvaders::Render() {
    glPointSize(4); // dimensiunea punctelor
    glBegin(GL_POINTS);
    for (auto& p : particles) {
        glColor3f(p.r, p.g, p.b);
        glVertex2f(p.x, p.y);
    }
    glEnd();
}