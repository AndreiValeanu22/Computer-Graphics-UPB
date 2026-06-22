#include "../include/gl_includes.h"
#include "../include/CIPlayer.h"
//#include <GL/gl.h>
#include <algorithm>
#include "../include/ChickenInvaders.h"

// CHICKEN INVADERS

CIPlayer::CIPlayer(float x_, float y_) : x(x_), y(y_) {}

void CIPlayer::Move(float dt, GLFWwindow* window) {
    shootCooldown -= dt; // scade cooldown tragere
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) x -= speed * dt;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) x += speed * dt;
    if (x < 0) x = 0;
    if (x + width > 800) x = 800 - width;
    /*if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && shootCooldown <= 0) {
        Shoot()
        shootCooldown = 0.2f; // 200 ms delay intre trageri.
    }*/
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) y += speed * dt;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) y -= speed * dt;
}

void CIPlayer::Shoot(const std::vector<Componenta>& nava) {
    float cx = 0, cy = 0;
    int count = 0;
    for (const auto& c : nava) {
        if (c.tip == TipComponenta::Tun) {
            // alegi coordonata capatului tunului
            cx += c.x + c.width / 2; // coordonata tun relativ
            cy += c.y + c.height; // sau varful tunului
            count++;
        }
    }
    if (count > 0) {
        cx /= count; // media tunurilor
        cy /= count; // media tunurilor
        projectiles.emplace_back(cx, cy, 400, true); // trageri din tunurile reale, nu central!
    }
    else {
        // fallback, eventual vechiul centru al navei
        projectiles.emplace_back(x + width / 2, y + height, 400, true); // fallback centru
    }

}

void CIPlayer::UpdateProjectiles(float dt) {
    for (auto& p : projectiles) p.Update(dt); // misca fiecare proiectil
    projectiles.erase(std::remove_if(projectiles.begin(), projectiles.end(), [](Projectile& p) 
        { return !p.alive; }), projectiles.end()); // sterge cele iesite din zona
}

void CIPlayer::Render() {
    // Corp nava (partea de sus)
    glColor3f(0.0f, 0.5f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height / 2);
    glVertex2f(x, y + height / 2);
    glEnd();

    // Tun central vizibil (desenat la centru)
    glColor3f(0.0f, 1.0f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(x + width / 2 - 5, y + height / 2);
    glVertex2f(x + width / 2 + 5, y + height / 2);
    glVertex2f(x + width / 2 + 5, y + height);
    glVertex2f(x + width / 2 - 5, y + height);
    glEnd();

    // Deseneaza toate proiectilele active
    for (auto& p : projectiles) p.Render();
}
