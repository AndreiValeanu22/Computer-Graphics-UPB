//#include <GL/gl.h>
#include "../include/gl_includes.h"
#include "../include/Pong.h"
#include <cmath>
// PONG
Ball::Ball(float  x_, float y_, float r, float s)
    : x(x_), y(y_), radius(r), speed(s) {
    vx = speed; // initializare viteza pe x
    vy = 0; // viteza pe y la start este zero
}

void Ball::Update(float dt) {
    x += vx * dt; // deplasare pe x
    y += vy * dt; // deplasare pe y

    // ciocnire cu marginea de sus sau jos
    if (y - radius < 0) { y = radius; vy = -vy; }
    if (y + radius > 600) { y = 600 - radius; vy = -vy; }
}

void Ball::Render() {
    glColor3f(1.0f, 0.6f, 0.1f); // culoarea portocalie pentru minge
    glBegin(GL_TRIANGLE_FAN); // desenare cerc
    for (int i = 0; i < 360; i++) {
        float theta = i * 3.14159f / 180.0f; // unghiul curent
        glVertex2f(x + radius * cos(theta), y + radius * sin(theta)); // pozitie pe cerc
    }
    glEnd();
}

void Ball::Reset(int direction) {
    x = 400; // centru pe x
    y = 300; // centru pe y
    speed = 300; // resetezi viteza la valoarea inițială! 
    // Asa am ales eu sa fac. Daca mingea porneste in dreapta, si paletele le lasi in pozitia initiala, pana cand mingea se plimba haotic stanga <-> dreapta,
	// punctul se pune la scorul player ului din stanga. Analog invers. Doar daca pierde player ul din stanga, punctul se pune la scorul
    // player ului din dreapta.
    // Daca direction = +1 porneste spre dreapta; daca -1 spre stanga
    vx = speed * direction;
    vy = 0;
}


void Ball::Bounce(float angle, int dirX) {
    speed += speedIncrement; // creste viteza
    vx = speed * cos(angle) * dirX; // calculeaza noua viteza pe x dupa ricoseu
    vy = speed * sin(angle); // calculeaza noua viteza pe y dupa ricoseu
}
