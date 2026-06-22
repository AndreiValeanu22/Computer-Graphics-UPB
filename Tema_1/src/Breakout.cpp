//#include <GL/gl.h>
#include "../include/gl_includes.h"
#include "../include/Breakout.h"
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
// BREAKOUT
// functie ajutatoare pentru coliziune cerc-dreptunghi
static bool CircleRectCollision(float cx, float cy, float radius, float rx, float ry, float rw, float rh, float& out_closest_x, float& out_closest_y) {
    float closestX = std::fmax(rx, std::fmin(cx, rx + rw)); // punctul cel mai apropiat pe x
    float closestY = std::fmax(ry, std::fmin(cy, ry + rh)); // punctul cel mai apropiat pe y
    out_closest_x = closestX; // output
    out_closest_y = closestY;
    float dx = cx - closestX;
    float dy = cy - closestY;
    return (dx * dx + dy * dy) <= (radius * radius); // returneaza daca e coliziune
}

Breakout::Breakout() : paddle(340, 30), score(0), lives(3), gameOver(false), ballOnPaddle(true) {}

void Breakout::Init() {
    bricks.clear();
    balls.clear();
    score = 0;
    lives = 3;
    gameOver = false;
    ballOnPaddle = true;
    particles.particles.clear();

    // setare grid de caramizi
    int cols = 8;
    int rows = 6;
    float gutter = 5.0f;
    float brickWidth = (800.0f - (cols - 1) * gutter) / cols;
    float brickHeight = 24.0f;
    float startY = 520.0f;

    // creeaza caramizile cu culoare/rezistenta diferita
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            int hits = 1 + (r % 7); // rezistenta si culoare
            Brick b(c * (brickWidth + gutter), startY - r * (brickHeight + 5), hits);
            b.width = brickWidth;
            b.height = brickHeight;
            bricks.push_back(b);
        }
    }

    ResetBall();
}

void Breakout::ResetBall() {
    balls.clear(); // sterge bilele vechi
    BreakoutBall bb(paddle.x + paddle.width * 0.5f, paddle.y + paddle.height + 8, 150 * ((rand() % 2) ? 1 : -1), 200);
    balls.push_back(bb);
    ballOnPaddle = true;
}

void Breakout::LaunchBall() {
    if (!ballOnPaddle) return;
    ballOnPaddle = false;
    // directionare random la lansare
    balls[0].vx = 150.0f * ((rand() % 2) ? 1 : -1);
    balls[0].vy = 220.0f;
}

void Breakout::CheckCollisions() {
    for (auto& ball : balls) {
        if (!ball.alive) continue;

        // coliziune cu paleta
        {
            float cx = ball.x, cy = ball.y;
            float px = paddle.x, py = paddle.y, pw = paddle.width, ph = paddle.height;
            float closeX, closeY;
            if (CircleRectCollision(cx, cy, ball.radius, px, py, pw, ph, closeX, closeY)) {
                // reflecta vertical si adauga deformare orizontala
                ball.y = py + ph + ball.radius + 0.1f; // muta mingea in afara
                ball.vy = fabs(ball.vy); // intoarce directia pe y
                float hitPos = (ball.x - paddle.x) / paddle.width - 0.5f;
                ball.vx += hitPos * 300.0f; // adauga deviatie pe x
                particles.Emit(ball.x, paddle.y + paddle.height * 0.5f + 5.0f, 10, 1.0f, 1.0f, 1.0f); // particule albe
            }
        }

        // coliziune cu caramizi
        for (auto& brick : bricks) {
            if (!brick.alive && !brick.destroying) continue;
            float closeX, closeY;
            if (CircleRectCollision(ball.x, ball.y, ball.radius, brick.x, brick.y, brick.width, brick.height, closeX, closeY)) {
                // se determina partea prin compararea pozitiei anterioare. Cum? simplu: inverseaza vy si ajusteaza usor vx
                float dx = ball.x - closeX;
                float dy = ball.y - closeY;
                if (std::fabs(dx) > std::fabs(dy)) {
                    // Impact orizontal (stanga/dreapta)
                    ball.vx = -ball.vx;
                }
                else {
                    // Impact vertical (sus/jos)
                    ball.vy = -ball.vy;
                }
                // Extra particule colorate la bricks rezistent
                if (brick.hits > 3) {
                    particles.Emit(ball.x, ball.y, 30, 1.0f, 0.2f, 0.6f); // roz/violet
                }
                brick.Hit();
                // culoare pentru particule in functie de rezistenta
                float pr = 1, pg = 1, pb = 1;
                int colorIndex = brick.hits + 1;
                switch (colorIndex) {
                case 1: pr = 0.0f; pg = 0.6f; pb = 1.0f; break;
                case 2: pr = 0.0f; pg = 1.0f; pb = 0.0f; break;
                case 3: pr = 1.0f; pg = 0.0f; pb = 0.0f; break;
                case 4: pr = 1.0f; pg = 1.0f; pb = 0.0f; break;
                case 5: pr = 0.6f; pg = 0.3f; pb = 0.0f; break;
                case 6: pr = 0.5f; pg = 0.5f; pb = 0.5f; break;
                default: pr = 0.6f; pg = 0.0f; pb = 0.6f; break;
                }
                particles.Emit(ball.x, ball.y, 14, pr, pg, pb);
                if (!brick.alive) {
                    shakeStrength = 12.0f; // intensitate
					shakeTimer = 0.30f;   // timp scurt // shake la distrugere brick, dar nu se prea vede in timpul jocului
                    score += 10;
                    // Multiball la fiecare 100 puncte
                    if (score > 0 && score % 100 == 0) { // la fiecare 100 puncte
                        balls.push_back(BreakoutBall(paddle.x + paddle.width / 2, paddle.y + paddle.height + 8, -150, 200));
                    }
                }
                else {
                    // scor partial la lovitura
                    score += 5;
                }
                break;
            }
        }

        // bila cazuta jos — pierde viata
        if (ball.y - ball.radius < 0) {
            ball.alive = false;
        }
    }

    /*
    // remove fully dead bricks after animation
    for (auto& brick : bricks) {
        brick.UpdateAnimation(0.0f); // noop unless destroying, but safe
    }*/

    // sterge bilele care au murit si vezi daca mai are viata
    bool anyAlive = false;
    for (auto& b : balls) if (b.alive) anyAlive = true;
    if (!anyAlive) {
        lives--;
        // Extra particule la pierdere viata
        for (int j = 0; j < 40; ++j)
            particles.Emit(paddle.x + paddle.width / 2, paddle.y, 1, 1.0f, 0.9f, 0.0f); // galben
        if (lives <= 0) {
            gameOver = true;
        }
        else {
            ResetBall();
        }
    }
}

void Breakout::Update(float dt) {
    if (gameOver) return; // nu actualiza jocul daca l-ai pierdut

    // update particule
    particles.Update(dt);

    // miscare paleta
    paddle.Move(dt, glfwGetCurrentContext());

    // lipeste bila de paleta la inceput
    if (ballOnPaddle && !balls.empty()) {
        balls[0].x = paddle.x + paddle.width * 0.5f;
        balls[0].y = paddle.y + paddle.height + balls[0].radius + 2.0f;
        if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_SPACE) == GLFW_PRESS) {
            LaunchBall();
        }
    }

    // update bile
    for (auto& b : balls) b.Update(dt);

    // update animatie brick
    for (auto& brick : bricks) {
        if (brick.destroying) brick.UpdateAnimation(dt);
    }

    // verifica coliziunile
    CheckCollisions();

    // sterge brick-urile care au disparut complet
    bricks.erase(std::remove_if(bricks.begin(), bricks.end(),
        [](const Brick& b) { return !b.alive && !b.destroying; }), bricks.end());

    // sterge bilele care au murit
    balls.erase(std::remove_if(balls.begin(), balls.end(),
        [](const BreakoutBall& b) { return !b.alive; }), balls.end());

    if (shakeTimer > 0.0f) {
        shakeTimer -= dt;
        if (shakeTimer < 0.0f) shakeTimer = 0.0f;
    }

}

void Breakout::Render() {
    // efect shake la distrugere brick
    if (shakeTimer > 0.0f) {
        float s = shakeStrength * (shakeTimer / 0.30f);
        float ox = (rand() % 201 - 100) / 100.0f * s;
        float oy = (rand() % 201 - 100) / 100.0f * s;
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glTranslatef(ox, oy, 0);
    }

    // background grid (optional)
    // draw paddle, bricks, balls, particles, UI
    paddle.Render();

    for (auto& brick : bricks) brick.Render(); // desen caramizi

    for (auto& ball : balls) ball.Render(); // desen bile

    particles.Render(); // desen particule

    // UI: scor si vieti
    glColor3f(1, 1, 1);
    glRasterPos2f(10, 580);
    char buf[80];
    sprintf(buf, "Score: %d   Lives: %d", score, lives);
    for (char* c = buf; *c; ++c) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);

    // UI suplimentar: afiseaza numarul de caramizi ramase
    int bricksLeft = 0;
    for (auto& b : bricks) if (b.alive || b.destroying) bricksLeft++;
    glRasterPos2f(340, 580);
    sprintf(buf, "Bricks left: %d", bricksLeft);
    for (char* c = buf; *c; ++c) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);

    // Highscore (static var simplu)
    static int highscore = 0;
    if (score > highscore) highscore = score;
    glRasterPos2f(600, 580);
    sprintf(buf, "Highscore: %d", highscore);
    for (char* c = buf; *c; ++c) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);

    // Game over UI

    if (gameOver) {
        glRasterPos2f(320, 300);
        const char* msg = "GAME OVER";
        for (const char* p = msg; *p; ++p) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *p);
    }

    if (shakeTimer > 0.0f) {
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
    }
}
