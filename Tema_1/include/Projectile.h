#pragma once
#include "../include/gl_includes.h"
// CHICKEN INVADERS

class Projectile { // clasa pentru proiectil (glont sau ou)
public:
    float x, y; // pozitia proiectilului
    float radius; // raza proiectilului (pentru coliziuni si desen)
    float speedY; // viteza verticala
    bool fromPlayer; // true = nava, false = inamic
                    // true daca e tras de jucator, false daca e de la inamic
    bool alive; // true daca proiectilul este activ

    Projectile(float x, float y, float speedY, bool fromPlayer); // constructor
    void Update(float dt); // actualizeaza pozitia in functie de timp
    void Render(); // deseneaza proiectilul pe ecran
};
