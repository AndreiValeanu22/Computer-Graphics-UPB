#pragma once
#include "Componenta.h"
#include <vector>
#include "../include/gl_includes.h"
#include <utility> // pentru std::pair

// EDITOR NAVA

class EditorNava {
public:
	std::vector<Componenta> componente; // Componentele adaugate in nava
	Componenta* selected = nullptr; // Componenta selectata (0 daca nu e nicio componenta selectata) pentru mutare
	bool startPressed = false; // Daca butonul Start a fost apasat

	const std::vector<Componenta>& GetVehicul() const { return componente; } // Returneaza componentele navei

	bool NavaValida(); // Verifica daca nava este valida (exista cel putin un corp si un motor)

	EditorNava(); // Constructor
	void Render(); // Functie de randare (OpenGL)
	void Update(float dt, GLFWwindow* window); // Functie de update (logica)
	void AddComponent(TipComponenta tip, float mx, float my); // Adauga o componenta in nava
	void RemoveSelected(); // Scoate componenta selectata din nava

private:
    // Grila MARE interactiva (mozaic)
    static constexpr int GRID_ROWS = 9;   // randuri pe placa mare
    static constexpr int GRID_COLS = 14;  // coloane pe placa mare

    // Geometria panoului central (ca in randare)
	float panelX = 180.0f, panelY = 60.0f, panelW = 600.0f, panelH = 520.0f; // panoul central
	float pad = 18.0f, gap = 6.0f;   // margine interna & spatiu intre tile-uri; tile = pătrat albastru din mozaic

    // Calculati in constructor
    float cellSize;                  // latura unui patrat albastru din mozaic
    float gridStartX, gridStartY;    // coltul stanga-jos al primului patrat

    bool grid[GRID_ROWS][GRID_COLS]; // matrice ocupare pentru mozaic

    bool leftWasPressed = false; // Starea butonului stang
	bool rightWasPressed = false; // Starea butonului drept
    bool dragging = false;       // Daca mutam o componenta din bara laterala
    TipComponenta draggingTip;   // Tipul componentei in drag
    double dragX = 0, dragY = 0; // Pozitia mouse-ului in timpul drag-ului

    std::vector<Componenta> grila; // Componentele din panoul lateral

    // Functii private declarate (folosite in EditorNava.cpp)
	void RenderGrid(); // Randare mozaic
	void RenderUI(); // Randare interfata (butoane, panou lateral)


	bool CanPlaceAt(TipComponenta tip, int row, int col); // Verifica daca o componenta de tipul dat poate fi plasata la pozitia data in mozaic
	std::vector<std::pair<int, int>> FootprintCells(TipComponenta tip, int baseRow, int baseCol); 
	// Returneaza celulele ocupate de o componenta de tipul dat la pozitia data
};