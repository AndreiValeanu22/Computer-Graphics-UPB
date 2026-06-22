#include "../include/gl_includes.h"
#include "../include/ChickenInvaders.h"
//#include <GL/gl.h>
#include <algorithm>
#include <cstdio>
#include <cmath>
#include "../include/ParticleSystem_ChickenInvaders.h"
#include <iostream>

// CHICKEN INVADERS
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

ChickenInvaders::ChickenInvaders() : player(400, 50), score(0), gameOver(false), spawnTimer(0), wave(0) {}

void ChickenInvaders::SetNava(const std::vector<Componenta>& nav) {
    // Salveaza local structura primita (recomandat e sa salvez o copie)
    nava = nav; // nava trebuie sa fie un membru std::vector<Componenta> in ChickenInvaders

    // SCALEZ TOATE componentele IMEDIAT DUPA preluare din editor:
    float scale = 0.3f; // 0.3f pot sa o modifc cu o alta valoare
    for (auto& c : nava) {
        c.x *= scale;
        c.y *= scale;
        c.width *= scale;
        c.height *= scale;
    }

    int numMotors = 0;
    for (const auto& c : nava)
        if (c.tip == TipComponenta::Motor) numMotors++;
    player.speed = 300 + 50 * numMotors;  // ADAPTAT: ajusteaza baza/multiplii dupa preferinta
}


void ChickenInvaders::Init() {
    enemies.clear();
    enemyProjectiles.clear();
    score = 0;
    gameOver = false;

    player.x = 400;
    player.y = 50; // valoreaza ca y sa fie 50 sau chiar mai mare (daca nava scaled e mica, poate fi 100)

    SpawnWave();
}


void ChickenInvaders::SpawnWave() {
    wave++;
    enemies.clear();
    for (int i = 0; i < 5 + wave; i++) {
        Enemy e(100 + i * 60, 500);
        e.speedY = 20 + wave * 5;        // mai rapizi pe verticala
        e.amplitude = 30 + wave * 5;     // oscilatie mai mare
        e.frequency = 1.5 + wave * 0.2f; // oscilatie mai rapida
        e.initialX = e.x;
        enemies.push_back(e);
    }

    spawnTimer = 5.0f;
}



void ChickenInvaders::CheckCollisions() {
    // proiectile nava vs inamici (coliziune cerc)
    for (auto& p : player.projectiles) {
        for (auto& e : enemies) {
            if (e.alive && p.alive) {
                float dx = p.x - (e.x + e.width / 2);
                float dy = p.y - (e.y + e.height / 2);
                float distance = sqrt(dx * dx + dy * dy);

                if (distance < p.radius + e.width / 2) {
                    e.health--;
                    p.alive = false;
                    if (e.health <= 0) {
                        e.alive = false;
                        score += 10;
                    }
                    particles.Emit(e.x + e.width / 2, e.y + e.height / 2, 18, 1.0f, 0.9f, 0.25f); // galben-portocaliu, ajusteaza nr/culoare dupa preferinta mea
                }
            }
        }
    }

    // proiectile inamici vs nava (coliziune dreptunghi)
    for (auto& p : enemyProjectiles) {
        if (player.alive && p.alive) {
            for (const auto& c : nava) {
                if (p.x > c.x + player.x && p.x < c.x + player.x + c.width &&
                    p.y > c.y + player.y && p.y < c.y + player.y + c.height) {
                    player.alive = false;
                    gameOver = true;
                }
            }
        }
    }


    // inamici ating marginea de jos → joc pierdut
    for (auto& e : enemies) {
        if (e.alive && e.y < 0) {
            gameOver = true;
        }
    }
}


void ChickenInvaders::Update(float dt) {
    particles.Update(dt);

    // for (auto& c : nava) c.Render();

    if (gameOver) return;

    GLFWwindow* window = glfwGetCurrentContext();

    // Cooldown tragere
    player.shootCooldown -= dt;

    // Mișcare jucator (pozitionare nava controlata)
    player.Move(dt, window);
    // if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_SPACE) == GLFW_PRESS)
    //    player.Shoot();

    // TRAGERE JUCATOR - FOLOSESTE NAVA
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && player.shootCooldown <= 0) {
        // Trage din fiecare tun
        // std::cout << "Numar tunuri: ";
        int tunuri = 0;
        for (const auto& c : nava) {
            if (c.tip == TipComponenta::Tun) {
                float x_shot = c.x + player.x + c.width / 2;
                float y_shot = c.y + player.y + c.height + 25; // ADAUGA offset ca sa nu traga in interiorul navei;
                                                               // incerc 25, nu 5, ca sa fie sigur vizibil deasupra navei
                player.projectiles.emplace_back(x_shot, y_shot, 400, true);
                tunuri++;
            }
        }
        // Daca nu ai tun, fallback la centrul navei vechi
        if (tunuri == 0) {
            player.projectiles.emplace_back(player.x + player.width / 2, player.y + player.height, 400, true);
        }
        player.shootCooldown = 0.2f;
        // std::cout << tunuri << "\n";
    }


    // Actualizare proiectile jucător
    player.UpdateProjectiles(dt);

    // Actualizare inamici + proiectile inamici
    for (auto& e : enemies) e.Shoot(enemyProjectiles, dt);
    for (auto& p : enemyProjectiles) p.Update(dt);

    // Verifica coliziuni proiectile/inamici
        CheckCollisions();

    /*for (auto& p : enemyProjectiles)
        p.Update(dt);

    // Elimina proiectile moarte
    enemyProjectiles.erase(std::remove_if(enemyProjectiles.begin(), enemyProjectiles.end(),
        [](Projectile& p) { return !p.alive; }), enemyProjectiles.end());

    // Verifica daca toti inamicii sunt distrusi pentru a crea urmatorul val
    if (std::all_of(enemies.begin(), enemies.end(), [](Enemy& e) { return !e.alive; })) {
        spawnTimer -= dt;
        if (spawnTimer <= 0) {
            SpawnWave();          // genereaza urmatorul val
            spawnTimer = 5.0f;    // 5 secunde pana la urmatorul val
        }
    }*/

    // Eliminare proiectile inamici inactive
    enemyProjectiles.erase(std::remove_if(enemyProjectiles.begin(), enemyProjectiles.end(),[](Projectile& p) { return !p.alive; }), enemyProjectiles.end());

    // Urmatorul val dacă toți inamicii au fost eliminati
    if (std::all_of(enemies.begin(), enemies.end(), [](Enemy& e) { return !e.alive; })) {
        spawnTimer -= dt;
        if (spawnTimer <= 0) {
            SpawnWave();
            spawnTimer = 5.0f;
        }
    }
}


void ChickenInvaders::Render() {
    for (auto& p : player.projectiles) p.Render();

    // Dacă exista componente in nava, deseneaza-le:
    if (!nava.empty()) {
        // Pozitioneaza toate componentele navei la (player.x, player.y)
        for (auto& c : nava) {
            // Da un offset la x, y pe baza pozitiei jucatorului fata de origine
            float x_draw = c.x + player.x;
            float y_draw = c.y + player.y;
            float w = c.width, h = c.height;
            TipComponenta t = c.tip;
            bool sel = c.selected;
            float r = c.r, g = c.g, b = c.b;

            // Creeaza o copie temporara cu offset; nu modifica c în vector
            Componenta draw_c(x_draw, y_draw, w, h, t);
            draw_c.selected = sel;
            draw_c.r = r; draw_c.g = g; draw_c.b = b;
            draw_c.Render();
        }
    }
    else {
        // fallback: daca nu ai nicio nava (nu ai construit nimic), apelezi player.Render()
        player.Render();
    }

    for (auto& e : enemies) e.Render();
    for (auto& p : enemyProjectiles) p.Render();

    // scor
    glColor3f(1, 1, 1);
    glRasterPos2f(10, 580);
    char buf[32]; sprintf(buf, "Score: %d", score);
    for (char* c = buf; *c; c++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);

    particles.Render();
}
