#pragma once
#include "Game.h"
#include "BreakoutBall.h"
#include "Brick.h"
#include "PaddleBreakout.h"
#include "ParticleSystem.h"
#include <vector>
//#include <GLFW/glfw3.h>
#include "../include/gl_includes.h" // gl_includes.h este un fisier care include GLFW/glfw3.h si alte antete OpenGL necesare
// BREAKOUT
class Breakout : public Game {
public:
	PaddleBreakout paddle; // reprezinta paleta jucatorului
	std::vector<BreakoutBall> balls; // reprezinta bilele in joc
	std::vector<Brick> bricks; // reprezinta caramizile de spart
	ParticleSystem particles; // sistemul de particule pentru efecte vizuale

	int score; // scorul jucatorului
	int lives; // numarul de vieti ramase
	bool gameOver; // indica daca jocul s-a terminat
	bool ballOnPaddle; // indica daca bila este pe paleta

	Breakout(); // constructor

	void Init() override; // initializeaza jocul
	void Update(float dt) override; // actualizeaza starea jocului
	void Render() override; // deseneaza jocul

private:
	void ResetBall(); // reseteaza pozitia bilei pe paleta
	void LaunchBall(); // lanseaza bila in joc
	void CheckCollisions(); // verifica coliziunile dintre bile, paleta si caramizi
	float shakeTimer = 0.0f; // timer pentru efectul de shake
	float shakeStrength = 0.0f; // intensitatea efectului de shake
};
