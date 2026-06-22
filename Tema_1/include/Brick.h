#pragma once
#include "../include/gl_includes.h"
//#include <GL/gl.h>
// BREAKOUT
class Brick {
public:
	float x, y; // pozitia coltului stanga-jos
	float width = 60, height = 20; // dimensiuni standard
	int hits;           // cate lovituri mai rezista ca sa fie distrusa caramida
    bool alive;         // inca activa (nu complet distrusa)
    bool destroying;    // in animatie de disparitie
    float destroyTimer; // timer animatie

	Brick(float x_ = 0, float y_ = 0, int hits_ = 1); // constructor care initializeaza pozitia si rezistenta
	void Hit();       // aplica o lovitura caramizii
	void Render();    // deseneaza caramida
	void UpdateAnimation(float dt);	// actualizeaza animatia de distrugere
};
