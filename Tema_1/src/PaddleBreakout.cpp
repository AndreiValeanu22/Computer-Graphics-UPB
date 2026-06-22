//#include <GL/gl.h>
#include "../include/gl_includes.h"
#include "../include/PaddleBreakout.h"
// BREAKOUT

PaddleBreakout::PaddleBreakout(float x_, float y_) : x(x_), y(y_) {}

void PaddleBreakout::Move(float dt, GLFWwindow* window) {
    if (!window) return;
    // Deplasare stanga/dreapta cu A/D sau sageti (alege ce preferi)
    // Am lasat sagetile, la fel ca in codul initial
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) x -= speed * dt;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) x += speed * dt;

    // Limitare la margini
    if (x < 0) x = 0;
    if (x + width > 800) x = 800 - width;
}

void PaddleBreakout::Render() {
    glColor3f(1.0f, 1.0f, 1.0f); // Paleta albă
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();
    // (codul pentru tun eliminat la Breakout
}