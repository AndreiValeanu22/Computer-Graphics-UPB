#pragma once
#include "Game.h"      // Game trebuie inclus primul
#include "PongPlayer.h"    // Player trebuie inclus inainte sa-l folosesti
#include "Ball.h"      // Ball pentru bile
#include <vector>      // pentru vector
// PONG
class Pong : public Game { // clasa pentru jocul Pong, mosteneste Game
private:
    PongPlayer player1, player2; // cei doi jucatori, stanga si dreapta
    std::vector<Ball> balls; // vector cu toate mingile active in joc
    int score1, score2; // scorul celor doi jucatori
    const int maxScore = 10; // scorul maxim la care se termina jocul

public:
    void Init() override; // initializeaza jocul
    void Update(float dt) override; // update logic pentru fiecare frame, cu dt 
    void Render() override; // update logic pentru fiecare frame, cu dt 
};
