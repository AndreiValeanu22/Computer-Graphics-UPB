#pragma once
#include <vector>
#include "Projectile.h"

// CHICKEN INVADERS

class Enemy {
public:
	float x, y; // pozitia centrului
	float width, height; // dimensiunile
    float speedX, speedY; // speedX poate fi folosit ca offset
	int health; // numarul de lovituri necesare pentru a-l distruge
	bool alive; // daca inamicul este viu
	float shootTimer; // temporizator pentru tragere

    // variabile sinus/coborâre
	float initialX; // pozitia initiala pe orizontala
	float amplitude; // amplitudinea miscarii sinusoidale
	float frequency; // frecventa miscarii sinusoidale
	float elapsedTime; // timpul scurs pentru miscarea sinusoidala

	Enemy(float x, float y, int health = 1); // constructor
	void Update(float dt); // actualizeaza pozitia si starea inamicului
	void Shoot(std::vector<Projectile>& enemyProjectiles, float dt); // inamicul trage proiectile
	void Render(); // deseneaza inamicul
};
