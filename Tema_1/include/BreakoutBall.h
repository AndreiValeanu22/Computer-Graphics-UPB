#include "../include/gl_includes.h"
//#include <GL/gl.h>
// BREAKOUT
class BreakoutBall {
public:
	float x, y; // reprezinta pozitia bilei
	float radius = 8; // raza bilei
	float vx, vy; // viteza bilei pe axele x si y
	bool alive = true; // indica daca bila este activa in joc

	BreakoutBall(float x_ = 0, float y_ = 0, float vx_ = 150, float vy_ = 200); // constructor pentru initializarea bilei
	void Update(float dt); // actualizeaza pozitia bilei in functie de timpul scurs
	void Render(); // deseneaza bila pe ecran
};
