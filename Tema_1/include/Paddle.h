#pragma once
#include "../include/gl_includes.h"
//#include <GL/gl.h>
// PONG
class Paddle {
public:
	float x, y; // pozitia paddle-ului
	float width = 120, height = 20; // dimensiunile paddle-ului
	float speed = 480; // viteza de miscare a paddle-ului

	Paddle(float x_ = 340, float y_ = 30); // constructor
	void Move(float dt, GLFWwindow* window); // misca paddle-ul in functie de input
	void Render(); // deseneaza paddle-ul
};
