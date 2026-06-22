//#include <GL/gl.h>
#include "../include/gl_includes.h"
#include "../include/Componenta.h"
#include <cmath>

// EDITOR NAVA

Componenta::Componenta(float x_, float y_, float w_, float h_, TipComponenta t)
    : x(x_), y(y_), width(w_), height(h_), tip(t) {
    // Stabilim culoarea default din tip componenta
    switch (tip) {
    case TipComponenta::Motor: r = 1; g = 0; b = 0; break; // Rosu motor
    case TipComponenta::Aripa: r = 0; g = 1; b = 0; break; // Verde aripa
    case TipComponenta::Tun: r = 0; g = 0; b = 1; break; // Albastru tun
    case TipComponenta::Corp: r = 0.8f; g = 0.8f; b = 0.8f; break; // Gri corp
    }
}

// verifica daca un punct (px, py) este in interiorul componentei (AABB)
bool Componenta::Contains(float px, float py) const {
    return (px >= x && px <= x + width && py >= y && py <= y + height);
}

void Componenta::Render() {
    glLineWidth(selected ? 3.0f : 1.0f); // selectie cu grosime diferita

    switch (tip) {
    case TipComponenta::Motor: {
        // desen motor ca "palarie" si picior
        glColor3f(0.9f, 0.9f, 0.2f); // galben
        float cx = x + width * 0.5f;
        float domeY = y + height * 0.72f;
        float rx = width * 0.48f;
        float ry = height * 0.36f;
        int seg = 32;

        // dom (semi-elipsa)
        glColor3f(0.95f, 0.93f, 0.82f);
        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(cx, domeY);
        for (int i = 0; i <= seg; i++) {
            float t = (float)i / seg * 3.1415926f; // 0..pi
            glVertex2f(cx + std::cos(t) * rx, domeY + std::sin(t) * ry);
        }
        glEnd();

        // picior motor
        float stemW = width * 0.32f;
        float stemH = height * 0.28f;
        float sx0 = cx - stemW * 0.5f;
        float sy0 = y + height * 0.08f;

        glColor3f(0.88f, 0.88f, 0.88f);
        glBegin(GL_QUADS);
        glVertex2f(sx0, sy0);
        glVertex2f(sx0 + stemW, sy0);
        glVertex2f(sx0 + stemW, sy0 + stemH);
        glVertex2f(sx0, sy0 + stemH);
        glEnd();

        // umbra sub dom (palarie)
        glColor3f(0.65f, 0.65f, 0.65f);
        glBegin(GL_QUADS);
        glVertex2f(sx0, sy0 + stemH - 2);
        glVertex2f(sx0 + stemW, sy0 + stemH - 2);
        glVertex2f(sx0 + stemW, sy0 + stemH);
        glVertex2f(sx0, sy0 + stemH);
        glEnd();
        break;
    }

    case TipComponenta::Tun: {
        // coloana intunecata + baza rotunjita
        glColor3f(0.2f, 1.0f, 0.2f); // verde
        float cx = x + width * 0.5f;
        float bodyW = width * 0.35f;
        float bodyH = height * 0.70f;
        float y0 = y + height * 0.18f;

        // corp
        glColor3f(0.10f, 0.10f, 0.12f);
        glBegin(GL_QUADS);
        glVertex2f(cx - bodyW * 0.5f, y0);
        glVertex2f(cx + bodyW * 0.5f, y0);
        glVertex2f(cx + bodyW * 0.5f, y0 + bodyH);
        glVertex2f(cx - bodyW * 0.5f, y0 + bodyH);
        glEnd();

        // baza semi-cerc (in jos)
        float rx = bodyW * 0.55f, ry = bodyW * 0.55f;
        float bcx = cx, bcy = y + height * 0.14f;
        int seg = 28;

        glColor3f(0.85f, 0.85f, 0.85f);
        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(bcx, bcy);
        for (int i = 0; i <= seg; i++) {
            float t = 3.1415926f + (float)i / seg * 3.1415926f; // pi..2pi
            glVertex2f(bcx + std::cos(t) * rx, bcy + std::sin(t) * ry);
        }
        glEnd();

        // inel luminos
        glColor3f(0.75f, 0.75f, 0.75f);
        glBegin(GL_QUADS);
        glVertex2f(cx - bodyW * 0.5f, y0);
        glVertex2f(cx + bodyW * 0.5f, y0);
        glVertex2f(cx + bodyW * 0.5f, y0 + 3);
        glVertex2f(cx - bodyW * 0.5f, y0 + 3);
        glEnd();
        break;
    }

    case TipComponenta::Aripa: {
        // „jet” portocaliu cu flacari zimtate
        glColor3f(1.0f, 0.7f, 0.3f); // portocaliu
        float bx0 = x + width * 0.34f;
        float bx1 = x + width * 0.66f;
        float by0 = y + height * 0.28f;
        float by1 = y + height * 0.82f;

        // corp jet
        glColor3f(1.00f, 0.58f, 0.00f);
        glBegin(GL_QUADS);
        glVertex2f(bx0, by0); glVertex2f(bx1, by0);
        glVertex2f(bx1, by1); glVertex2f(bx0, by1);
        glEnd();

        // flacari
        glColor3f(1.00f, 0.35f, 0.00f);
        int teeth = 5;
        float toothW = (bx1 - bx0) / teeth;
        for (int i = 0; i < teeth; i++) {
            float tx0 = bx0 + i * toothW;
            float tx1 = tx0 + toothW;
            glBegin(GL_TRIANGLES);
            glVertex2f(tx0, by0);
            glVertex2f(tx1, by0);
            glVertex2f(tx0 + toothW * 0.5f, by0 - height * 0.18f);
            glEnd();
        }

        // capac superior
        glColor3f(1.00f, 0.68f, 0.15f);
        glBegin(GL_QUADS);
        glVertex2f(bx0, by1);
        glVertex2f(bx1, by1);
        glVertex2f(bx1, by1 + height * 0.06f);
        glVertex2f(bx0, by1 + height * 0.06f);
        glEnd();
        break;
    }

    case TipComponenta::Corp: {
        // bloc metalic cu bevel
        glColor3f(0.6f, 0.6f, 1.0f); // albastru
        glColor3f(0.62f, 0.64f, 0.67f);
        glBegin(GL_QUADS);
        glVertex2f(x, y); glVertex2f(x + width, y);
        glVertex2f(x + width, y + height); glVertex2f(x, y + height);
        glEnd();

        float inset = width * 0.15f;
        glColor3f(0.80f, 0.82f, 0.86f);
        glBegin(GL_QUADS);
        glVertex2f(x + inset, y + inset);
        glVertex2f(x + width - inset, y + inset);
        glVertex2f(x + width - inset, y + height - inset);
        glVertex2f(x + inset, y + height - inset);
        glEnd();

        glColor3f(0.45f, 0.47f, 0.50f);
        glBegin(GL_TRIANGLES);
        glVertex2f(x + width - inset, y + inset);
        glVertex2f(x + width - inset, y + height - inset);
        glVertex2f(x + width, y + height);
        glEnd();
        break;
    }
    } // switch

    // contur selectie
    if (selected) {
        glColor3f(1, 1, 1);
        glLineWidth(3.0f);
        glBegin(GL_LINE_LOOP);
        glVertex2f(x, y);
        glVertex2f(x + width, y);
        glVertex2f(x + width, y + height);
        glVertex2f(x, y + height);
        glEnd();
    }
}
