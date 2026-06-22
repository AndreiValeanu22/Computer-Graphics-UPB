#pragma once
// PONG
class PongPlayer {
public:
    float x, y; // pozitia coltului stanga-jos al paletei
    float width, height; // dimensiunile paletei
    float speed; // viteza de miscare pe verticala
    bool animating = false; // daca paleta este in animatie de ricoseu
    float animTime = 0.0f; // cat timp a trecut din animatie
    float animDuration = 0.2f; // durata intreaga a animatiei
    float animScale = 1.2f; // factorul de scalare pentru animatie (marire temporara)

    PongPlayer(float x = 0, float y = 0, float w = 20, float h = 100, float s = 400); // constructor, initializeaza toti parametrii

    void MoveUp(float dt); // muta paleta in sus
    void MoveDown(float dt); // muta paleta in jos
    void Render(float r, float g, float b); // deseneaza paleta cu culoare data
    void Animate(float dt); // actualizeaza starea animatiei daca paleta a deviat mingea
    void TriggerAnimation(); // declanseaza animatia de ricoseu
};
