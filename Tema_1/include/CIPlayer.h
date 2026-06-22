#pragma once
#include <vector>
#include "Projectile.h"
#include "Componenta.h"
// CHICKEN INVADERS
class CIPlayer {
public:
	float x, y; // pozitia centrului jucatorului
	float width = 50, height = 30; // dimensiunile jucatorului
	float speed = 300; // viteza de miscare pe orizontala
	float shootCooldown = 0.0f; // initializezi cu 0 pentru a putea trage imediat. shootCooldown reprezinta timpul ramas pana la urmatorul foc
	bool alive = true; // starea jucatorului
	std::vector<Projectile> projectiles; // proiectilele trase de jucator

	CIPlayer(float x = 0, float y = 0); // constructor cu valori implicite pentru pozitie
	void Move(float dt, GLFWwindow* window); // misca jucatorul in functie de input si delta time
	void Shoot(const std::vector<Componenta>& nava); // trage un proiectil daca cooldown-ul a expirat
	void UpdateProjectiles(float dt); // actualizeaza pozitia proiectilelor si elimina cele care ies din ecran
	void Render(); // deseneaza jucatorul si proiectilele
};
