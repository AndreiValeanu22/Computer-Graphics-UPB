//#include <GL/gl.h>
#include "../include/gl_includes.h"
#include "../include/Paddle.h"
// PONG

Paddle::Paddle(float x_, float y_) : x(x_), y(y_) {}

void Paddle::Move(float dt, GLFWwindow* window) {
    if (!window) return;
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) x -= speed * dt;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) x += speed * dt;

    if (x < 0) x = 0;
    if (x + width > 800) x = 800 - width;
}

void Paddle::Render() {
    glColor3f(1.0f, 1.0f, 1.0f); // alb
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();

    // desen mic "tun" verde în centru
    glColor3f(0.0f, 0.8f, 0.0f);
    float tx = x + width / 2 - 6;
    glBegin(GL_QUADS);
    glVertex2f(tx, y + height);
    glVertex2f(tx + 12, y + height);
    glVertex2f(tx + 12, y + height + 10);
    glVertex2f(tx, y + height + 10);
    glEnd();
}