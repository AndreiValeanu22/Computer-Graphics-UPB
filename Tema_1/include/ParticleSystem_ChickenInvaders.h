#pragma once
#include "../include/gl_includes.h"
#include <vector>

// CHICKEN INVADERS

struct Particle { // structura unei singure particule
	float x, y, vx, vy, r, g, b, a, lifetime;
    // vx si vy viteza de deplasare pe axele X și Y (velocity: cat se misca pe secunda)
    // vx si vy reprezinta "velocity" — adica rapiditatea si directia in care se deplaseaza particula.
    // r, g, b culoarea particulei (rosu, verde, albastru)
    // a transparenta (alpha) a particulei, pentru efect de fade
    // lifetime cat timp mai supravietuieste particula (in secunde)
};
class ParticleSystem_ChickenInvaders {
public:
    std::vector<Particle> particles; // vector cu toate particulele active din sistem
    // Creeaza nr particule la coordonatele (x, y), cu culoarea (r, g, b)
    void Emit(float x, float y, int nr, float r, float g, float b);
    // Actualizeaza pozitia si durata de viata a fiecarei particule pe baza vitezei si timpului dt
    void Update(float dt);
    // Deseneaza toate particulele pe ecran folosind OpenGL
    void Render();
};
/*
Velocity reprezinta viteza de deplasare a particulei pe fiecare axa:

vx = cat creste/scade poziția x pe secunda (pixeli/secunda)

vy = cat creste/scade pozitia y pe secunda
*/