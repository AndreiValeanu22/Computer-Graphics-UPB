#pragma once
#include "../include/gl_includes.h"
//#include <GL/gl.h>
// BREAKOUT
class PaddleBreakout { // Nume nou de clasa
public:
	float x, y; // Pozitia initiala a paletei
	float width = 120, height = 20; // Dimensiunile paletei
	float speed = 480; // Viteza de miscare a paletei

	PaddleBreakout(float x_ = 340, float y_ = 30); // Constructor cu valori implicite pentru pozitia initiala
	void Move(float dt, GLFWwindow* window); // Metoda pentru a misca paleta in functie de input si delta time
	void Render(); // Metoda pentru a desena paleta
};