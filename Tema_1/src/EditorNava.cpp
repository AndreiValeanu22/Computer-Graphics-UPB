//#include <GL/gl.h>
#include "../include/gl_includes.h"
#include "../include/EditorNava.h"
#include <cstdio>
#include <cmath>
#include <cstring>
#include <algorithm>
#include <functional> // Pentru std::function in NavaValida

// EDITOR NAVA

// Mapare coordonate mouse (în coordonate OpenGL, cu Y în sus) -> celulă din mozaic
static bool MouseToCell(double mx, double my,
    float gridStartX, float gridStartY,
    float cell, float gap,
    int rows, int cols,
    int& outRow, int& outCol)
{
    // translateaza la originea mozaicului
    double lx = mx - gridStartX;
    double ly = my - gridStartY;
    if (lx < 0 || ly < 0) return false;

    // indice „grosier” de celula
    outCol = (int)(lx / (cell + gap));
    outRow = (int)(ly / (cell + gap));
    if (outCol < 0 || outCol >= cols || outRow < 0 || outRow >= rows) return false;

    // verifica daca click-ul e chiar în „patratel” (nu in spatiul gap)
    double offx = lx - outCol * (cell + gap);
    double offy = ly - outRow * (cell + gap);
    if (offx > cell || offy > cell) return false;  // e pe gap

    return true;
}


// EDITOR NAVA

// functie ajutatoare text using GLUT 
// Aceasta functie inlocuieste implementarea manuala si incompleta a DrawTextMono
void DrawUIText(float x, float y, const char* text) {
    glColor3f(1, 1, 1); // Text alb
    glRasterPos2f(x, y);
    for (const char* c = text; *c; ++c) {
        // Folosim un font standard GLUT (de ex. Helvetica 12)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
    }
}

// Bara de sus: patratele verzi + steag verde (decorativ)
static void DrawTopHud(int numComponente, bool valid) {
    float sx = 200.0f, sy = 560.0f, w = 22.0f, gap = 16.0f;
    int restante = 10 - numComponente;
    // Desenam DOAR patratele verzi rămase
    for (int i = 0; i < restante; i++) {
        float x = sx + i * (w + gap);
        glColor3f(0.15f, 0.85f, 0.25f); // Verde
        glBegin(GL_QUADS);
        glVertex2f(x, sy);
        glVertex2f(x + w, sy);
        glVertex2f(x + w, sy + w);
        glVertex2f(x, sy + w);
        glEnd();
    }

    // Steag/bariera in dreapta - verde daca valid, roșu daca nu
    float fx = 760.0f, fy = 560.0f, fw = 24.0f, fh = 24.0f;
    if (valid)
        glColor3f(0.15f, 0.85f, 0.25f);  // verde
    else
        glColor3f(0.7f, 0.07f, 0.07f);   // rosu

    // Patrat capat
    glBegin(GL_QUADS);
    glVertex2f(fx, fy);
    glVertex2f(fx + fw, fy);
    glVertex2f(fx + fw, fy + fh);
    glVertex2f(fx, fy + fh);
    glEnd();

    // Triunghi extins din dreapta sus/cadre jos
    glBegin(GL_TRIANGLES);
    glVertex2f(fx + fw, fy);
    glVertex2f(fx + fw, fy + fh);
    glVertex2f(fx + fw + fw * 0.9f, fy + fh * 0.5f);
    glEnd();

}

// Panou central: cadru albastru + patratele albastre (mozaic)
static void DrawBackgroundPanel(
    float panelX, float panelY, float panelW, float panelH,
    float gridStartX, float gridStartY, int cols, int rows,
    float cell, float gap)
{
    // cadru
    glColor3f(0.05f, 0.12f, 0.30f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(panelX, panelY);
    glVertex2f(panelX + panelW, panelY);
    glVertex2f(panelX + panelW, panelY + panelH);
    glVertex2f(panelX, panelY + panelH);
    glEnd();

    // mozaic
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            float px = gridStartX + c * (cell + gap);
            float py = gridStartY + r * (cell + gap);

            glColor3f(0.12f, 0.52f, 0.98f);
            glBegin(GL_QUADS);
            glVertex2f(px, py);
            glVertex2f(px + cell, py);
            glVertex2f(px + cell, py + cell);
            glVertex2f(px, py + cell);
            glEnd();

            glColor3f(0.05f, 0.05f, 0.08f);
            glBegin(GL_LINE_LOOP);
            glVertex2f(px, py);
            glVertex2f(px + cell, py);
            glVertex2f(px + cell, py + cell);
            glVertex2f(px, py + cell);
            glEnd();
        }
    }
}

// Constructor
EditorNava::EditorNava() {
    float startX = 10, startY = 500, size = 50, gap = 10;

    // Initializare grila laterala (componente ce pot fi drag-uite) — doar pentru Breakout
    TipComponenta tipuri[4] = { TipComponenta::Corp, TipComponenta::Motor, TipComponenta::Aripa, TipComponenta::Tun };
    for (int i = 0; i < 4; i++) {
        Componenta c(startX, startY - i * (size + gap), size, size, tipuri[i]);
        // Tunul (TipComponenta::Tun) este 1x3, deci trebuie sa-i ajustam inaltimea
        if (tipuri[i] == TipComponenta::Tun) {
            c.height = size * 3.0f;
            // Ajustam pozitia Y pentru a arata corect in paleta
            c.y = startY - i * (size + gap) - size * 2.0f;
        }
        grila.push_back(c);
    }


    // Calcule pentru mozaicul interactiv
    cellSize = (panelW - 2 * pad - (GRID_COLS - 1) * gap) / GRID_COLS;
    gridStartX = panelX + pad;
    gridStartY = panelY + pad;

    // Initializare matrice de ocupare a grilei centrale la false
    memset(grid, 0, sizeof(grid));
}

// Render grid
void EditorNava::RenderGrid() {
    // DOAR paleta (componentele din stânga) + etichete
    for (auto& c : grila) c.Render();

    DrawUIText(10, 560, "Paleta Componente:");
    DrawUIText(70, 505, "Corp (1x1)");
    DrawUIText(70, 445, "Motor (1x1)");
    DrawUIText(70, 385, "Aripa (1x1)");
    DrawUIText(70, 265, "Tun (1x3)");
}


// Render UI
void EditorNava::RenderUI() {
    char buf[80];

    // Afisare Statistica
    sprintf(buf, "Componente: %d", (int)componente.size());
    DrawUIText(10, 580, buf);

    // Buton START
    float bx = 10, by = 50, bw = 80, bh = 30;
    bool valid = NavaValida();

    glColor3f(valid ? 0.1f : 0.7f, valid ? 0.87f : 0.07f, valid ? 0.18f : 0.07f); // Verde daca valid, rosu daca nu
    glBegin(GL_QUADS);
    glVertex2f(bx, by);
    glVertex2f(bx + bw, by);
    glVertex2f(bx + bw, by + bh);
    glVertex2f(bx, by + bh);
    glEnd();

    // Text Buton
    DrawUIText(bx + 20, by + 12, "START");

    if (startPressed) {
        glColor3f(0.0f, 0.8f, 0.0f);
        DrawUIText(bx + bw + 10, by + 12, "Lansare!");
    }

    // Bara de Stare de Sus (similar cu imaginea)
    glColor3f(0.0f, 0.5f, 0.0f); // Verde inchis
    glBegin(GL_QUADS);
    glVertex2f(180, 580);
    glVertex2f(795, 580);
    glVertex2f(795, 595);
    glVertex2f(180, 595);
    glEnd();

}
// Render (Adaugam bordurile rosii)
void EditorNava::Render() {
    glClearColor(0.07f, 0.07f, 0.07f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION); glLoadIdentity();
    glOrtho(0, 800, 0, 600, -1, 1);
    glMatrixMode(GL_MODELVIEW); glLoadIdentity();

    // 1) Borduri roșii subtile (exterior + separare paletă/canvas)
    glLineWidth(2.0f);
    glColor3f(1.0f, 0.0f, 0.0f);

    // contur mare
    glBegin(GL_LINE_LOOP);
    glVertex2f(5, 5); glVertex2f(795, 5);
    glVertex2f(795, 595); glVertex2f(5, 595);
    glEnd();

    // paleta stanga
    glBegin(GL_LINE_LOOP);
    glVertex2f(5, 5); glVertex2f(160, 5);
    glVertex2f(160, 595); glVertex2f(5, 595);
    glEnd();

    // separare paleta–canvas
    glBegin(GL_LINES);
    glVertex2f(160, 5); glVertex2f(160, 595);
    glEnd();

    // 2) Panoul + mozaicul mare interactiv (folosim membrii clasei)
    DrawBackgroundPanel(panelX, panelY, panelW, panelH,
        gridStartX, gridStartY, GRID_COLS, GRID_ROWS,
        cellSize, gap);

    // 3) Paleta din stanga (fara grila 5x5)
    RenderGrid();

    // 4) UI (buton START, text, etc.) – in tema albastra
    RenderUI();

    // 5) Bara de sus (HUD) cu patratele verzi + steag (ca in GIF)
    DrawTopHud((int)componente.size(), NavaValida());


    // 6) Componente plasate pe mozaic
    for (auto& c : componente) c.Render();

    // 7) Preview in timpul drag & drop – accent albastru translucid
    if (dragging) {
        int row, col;
        if (MouseToCell(dragX, dragY, gridStartX, gridStartY, cellSize, gap,
            GRID_ROWS, GRID_COLS, row, col)) {

            auto cells = FootprintCells(draggingTip, row, col);

            int minR = GRID_ROWS, minC = GRID_COLS, maxR = 0, maxC = 0;
            bool first = true;
            for (auto& p : cells) {
                if (first) { minR = maxR = p.first; minC = maxC = p.second; first = false; }
                minR = std::min(minR, p.first); minC = std::min(minC, p.second);
                maxR = std::max(maxR, p.first); maxC = std::max(maxC, p.second);
            }

            float bx = gridStartX + minC * (cellSize + gap);
            float by = gridStartY + minR * (cellSize + gap);
            float bw = (maxC - minC + 1) * cellSize + (maxC - minC) * gap;
            float bh = (maxR - minR + 1) * cellSize + (maxR - minR) * gap;

            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            bool canPlace = CanPlaceAt(draggingTip, row, col);
            glColor4f(0.00f, 0.60f, 1.00f, canPlace ? 0.28f : 0.12f);

            glBegin(GL_QUADS);
            glVertex2f(bx, by); glVertex2f(bx + bw, by);
            glVertex2f(bx + bw, by + bh); glVertex2f(bx, by + bh);
            glEnd();

            glDisable(GL_BLEND);
        }
    }
}


// Footprint (Forma Componentei)
std::vector<std::pair<int, int>> EditorNava::FootprintCells(TipComponenta tip, int baseRow, int baseCol) {
    std::vector<std::pair<int, int>> r;
    // baseRow/baseCol = celula din grila unde este pozitionat coltul (0,0) al componentei
    // NOTE: Randurile cresc in sus (0=jos, 4=sus)

    if (tip == TipComponenta::Corp || tip == TipComponenta::Aripa) {
        // Corp / Aripa (1x1)
        r.emplace_back(baseRow, baseCol);
    }
    else if (tip == TipComponenta::Motor) {
        // Motor (1x1)
        r.emplace_back(baseRow, baseCol);
    }
    else if (tip == TipComponenta::Tun) {
        // Tun (1x3, orientat pe coloana in sus)
        r.emplace_back(baseRow, baseCol);
        r.emplace_back(baseRow + 1, baseCol);
        r.emplace_back(baseRow + 2, baseCol);
    }
    return r;
}

// CanPlaceAt (Verificare reguli de plasare)
bool EditorNava::CanPlaceAt(TipComponenta tip, int row, int col) {
    auto cells = FootprintCells(tip, row, col);

    for (auto& p : cells) {
        int r = p.first, c = p.second;
        if (r < 0 || r >= GRID_ROWS || c < 0 || c >= GRID_COLS) return false;
        if (grid[r][c]) return false;
    }

    /*if (tip == TipComponenta::Aripa) {
        int dr[4] = { 0, 0, 1, -1 };
        int dc[4] = { 1, -1, 0, 0 };
        for (int k = 0; k < 4; k++) {
            int nr = row + dr[k], nc = col + dc[k];
            if (nr >= 0 && nr < GRID_ROWS && nc >= 0 && nc < GRID_COLS) {
                if (grid[nr][nc]) return false;
            }
        }
    }*/

    if (tip == TipComponenta::Motor) {
        for (int r = 0; r < row; r++) if (grid[r][col]) return false;
    }

    if (tip == TipComponenta::Tun) {
        int topRow = row + 2;
        for (int r = topRow + 1; r < GRID_ROWS; r++) if (grid[r][col]) return false;
    }
    return true;
}

// AddComponent (Adaugarea efectiva in grila)
void EditorNava::AddComponent(TipComponenta tip, float mx, float my) {
    int row, col;
    if (componente.size() >= 10) return; // <-- LIMITA DE 10
    if (!MouseToCell(mx, my, gridStartX, gridStartY, cellSize, gap, GRID_ROWS, GRID_COLS, row, col))
        return;

    if (!CanPlaceAt(tip, row, col)) return;

    auto cells = FootprintCells(tip, row, col);

    float renderX = gridStartX + col * (cellSize + gap);
    float renderY = gridStartY + row * (cellSize + gap);
    float compWidth = cellSize;
    float compHeight = (tip == TipComponenta::Tun) ? (cellSize * 3 + gap * 2) : cellSize;

    componente.emplace_back(renderX, renderY, compWidth, compHeight, tip);

    for (auto& p : cells) {
        int r = p.first, c = p.second;
        if (r >= 0 && r < GRID_ROWS && c >= 0 && c < GRID_COLS) grid[r][c] = true;
    }

    selected = &componente.back();
    selected->selected = true;
}

// RemoveSelected
void EditorNava::RemoveSelected() {
    if (!selected) return;

    for (auto it = componente.begin(); it != componente.end(); ++it) {
        if (&(*it) == selected) {
            int col = (int)std::lround((it->x - gridStartX) / (cellSize + gap));
            int row = (int)std::lround((it->y - gridStartY) / (cellSize + gap));

            auto cells = FootprintCells(it->tip, row, col);
            for (auto& p : cells) {
                int r = p.first, c = p.second;
                if (r >= 0 && r < GRID_ROWS && c >= 0 && c < GRID_COLS) grid[r][c] = false;
            }
            componente.erase(it);
            selected = nullptr;
            break;
        }
    }
}

// NavaValida (Verificare Conectivitate DFS)
bool EditorNava::NavaValida() {
    if (componente.empty()) return true;

    bool ocup[GRID_ROWS][GRID_COLS] = { 0 };
    int startR = -1, startC = -1;

    for (auto& c : componente) {
        int col = (int)std::lround((c.x - gridStartX) / (cellSize + gap));
        int row = (int)std::lround((c.y - gridStartY) / (cellSize + gap));

        auto cells = FootprintCells(c.tip, row, col);
        for (auto& p : cells) {
            int r = p.first, cc = p.second;
            if (r < 0 || r >= GRID_ROWS || cc < 0 || cc >= GRID_COLS) return false;
            if (ocup[r][cc]) return false;
            ocup[r][cc] = true;
            if (startR == -1) { startR = r; startC = cc; }
        }
    }
    if (startR == -1) return false;

    bool viz[GRID_ROWS][GRID_COLS] = { 0 };
    std::function<void(int, int)> dfs = [&](int r, int c) {
        viz[r][c] = true;
        int dr[4] = { -1, 1, 0, 0 }, dc[4] = { 0, 0, -1, 1 };
        for (int k = 0; k < 4; k++) {
            int nr = r + dr[k], nc = c + dc[k];
            if (nr >= 0 && nr < GRID_ROWS && nc >= 0 && nc < GRID_COLS && ocup[nr][nc] && !viz[nr][nc]) {
                dfs(nr, nc);
            }
        }
        };
    dfs(startR, startC);

    for (int r = 0; r < GRID_ROWS; r++)
        for (int c = 0; c < GRID_COLS; c++)
            if (ocup[r][c] && !viz[r][c]) return false;

    return true;
}


// Update (Logica mouse)
void EditorNava::Update(float dt, GLFWwindow* window) {
    if (!window) return;
    double mx, my;
    glfwGetCursorPos(window, &mx, &my);
    my = 600 - my; // Inversam Y (coordonate OpenGL)

    bool left = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
    bool right = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS;

    // TASTE
    if (glfwGetKey(window, GLFW_KEY_DELETE) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_BACKSPACE) == GLFW_PRESS) {
        RemoveSelected();
    }

    // CLICK DREAPTA (STERGERE/DESELECTARE)
    if (right && !rightWasPressed) { // Adaugat !rightWasPressed pentru a preveni stergerea continua
        // Stergere (daca s-a apasat pe o componenta)
        bool removed = false;
        for (auto it = componente.begin(); it != componente.end(); ++it) {
            if (it->Contains((float)mx, (float)my)) {
                selected = &(*it);
                RemoveSelected(); // Functia RemoveSelected face de-selectia si stergerea din grila
                removed = true;
                break;
            }
        }
        if (!removed && selected) {
            selected->selected = false;
            selected = nullptr;
        }
    }

    // CLICK STANGA (LOGICA PRINCIPALA)
    if (left && !leftWasPressed) {
        // 1. Initializare Drag (Click pe paleta)
        for (auto& c : grila) {
            if (c.Contains((float)mx, (float)my)) {
                dragging = true;
                draggingTip = c.tip;
                dragX = mx; dragY = my;
                if (selected) selected->selected = false; // Deselectam orice era selectat
                selected = nullptr;
                return;
            }
        }

        // 2. Click pe Butonul START
        if (mx >= 10 && mx <= 90 && my >= 50 && my <= 80 && NavaValida()) {
            startPressed = true;
        }

        // 3. Selectare/Mutare Componenta Existenta
        bool componentClicked = false;
        if (!dragging) {
            if (selected) selected->selected = false; // Deselectam vechea selectie
            selected = nullptr;
            for (auto& c : componente) {
                if (c.Contains((float)mx, (float)my)) {
                    selected = &c;
                    c.selected = true;
                    componentClicked = true;
                    // Logica de mutare/drag a componentei existente ar incepe aici
                    break;
                }
            }
        }

    }

    if (left && leftWasPressed) {
        // 4. In timpul Drag-ului sau Mutarii
        if (dragging) {
            dragX = mx;
            dragY = my;
        }
        // Pastram logica de mutare dezactivata, deoarece complica actualizarea grilei.
    }

    if (!left && leftWasPressed) {
        // 5. Finalizare Click Stanga (Release)

        // Finalizare Drag (Plasare)
        if (dragging) {
            // mx/my = pozitia de release, pe care o trimitem la AddComponent
            AddComponent(draggingTip, (float)mx, (float)my);
            dragging = false;
        }

        startPressed = false;
    }

    // Resetare stare butoane mouse (Actualizare la final)
    leftWasPressed = left;
    rightWasPressed = right; // Presupunand ca "rightWasPressed" exista ca membru privat
}