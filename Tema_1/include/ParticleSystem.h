#pragma once
#include "../include/gl_includes.h"
#include <vector>
//#include <GL/gl.h>
// BREAKOUT
struct PS_Particle { // strctura particulei
	float x, y; // pozitia
	float vx, vy; // viteza
	float lifetime; // durata de viata
	float r, g, b; // culoare
};

class ParticleSystem {
public:
	std::vector<PS_Particle> particles; // vector de particule
	void Emit(float x, float y, int count, float r, float g, float b); // emite particule
	void Update(float dt); // actualizeaza particulele
	void Render(); // deseneaza particulele
};
