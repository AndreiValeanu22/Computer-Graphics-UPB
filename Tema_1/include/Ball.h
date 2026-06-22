#pragma once
// PONG
class Ball {
public:
	float x, y; // pozitia bilei
	float radius; // raza bilei
	float vx, vy; // viteza bilei pe axele x si y
	float speed; // viteza initiala a bilei
    float speedIncrement = 20.0f; // creștere la fiecare ricoseu

	Ball(float x = 0, float y = 0, float r = 10, float s = 300); // constructor care initializeaza pozitia, raza si viteza bilei, 
	// cu valori implicite pentru a permite crearea unei bile fara parametri

	void Update(float dt); // actualizeaza pozitia bilei in functie de viteza si timpul scurs
	void Render(); // deseneaza bila pe ecran
	void Reset(int direction); // reseteaza pozitia si viteza bilei
    void Bounce(float angle, int dirX); // functie noua pentru ricoseu
};
