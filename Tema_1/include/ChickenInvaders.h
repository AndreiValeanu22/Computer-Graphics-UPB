#pragma once
#include "../include/ParticleSystem_ChickenInvaders.h"
#include "Game.h"
#include "CIPlayer.h"
#include "Enemy.h"
#include "Projectile.h"
#include <vector>
#include "Componenta.h"


// CHICKEN INVADERS
class ChickenInvaders : public Game {
private:
	CIPlayer player; // jucatorul
	std::vector<Enemy> enemies; // inamicii
	std::vector<Projectile> enemyProjectiles; // proiectilele inamicilor
	int score; // scorul jucatorului
	bool gameOver; // starea jocului
	float spawnTimer; // timer pentru spawn inamici
	int wave; // valorea valului curent

	std::vector<Componenta> nava; // componentele navei
	ParticleSystem_ChickenInvaders particles; // sistemul de particule

public:
	ChickenInvaders(); // constructor
	void Init() override; // inițializare joc
	void Update(float dt) override; // actualizare stare joc
	void Render() override; // randare joc, inclusiv particule; a randa inseamna a desena pe ecran

	void SetNava(const std::vector<Componenta>& nav); // seteaza componentele navei

private:
	void SpawnWave(); // spawn un val de inamici
	void CheckCollisions(); // verifica coliziunile dintre proiectile si inamici/jucator
};
