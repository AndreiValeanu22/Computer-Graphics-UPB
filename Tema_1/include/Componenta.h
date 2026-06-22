#pragma once
#include <vector>

// EDITOR NAVA

// Definim tipurile de componente
enum class TipComponenta {
    Corp,   // 1x1
    Motor,  // 1x1
    Aripa,  // 1x1
    Tun     // 1x3
};

class Componenta {
public:
	float x, y; // Pozitia in cadrul editorului
	float width, height; // Dimensiunile componentei
	TipComponenta tip; // Tipul componentei
	bool selected = false; // Starea de selectie

	// Culori RGB (valori intre 0 si 1) pentru fiecare tip de componenta; RGB vine de la Red, Green, Blue
	float r, g, b; // Culorile componentei

	Componenta(float x_, float y_, float w_, float h_, TipComponenta t); // Constructor

    // Functie de randare (OpenGL)
    void Render();

    // Functie necesara pentru detectia click-urilor
    bool Contains(float px, float py) const;
};