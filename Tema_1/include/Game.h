#pragma once

class Game {
public:
	virtual void Init() = 0; // Init initializeaza jocul
	virtual void Update(float dt) = 0; // Update actualizeaza starea jocului
	virtual void Render() = 0; // Render deseneaza jocul pe ecran
	virtual ~Game() {} // ~Game destructor virtual pentru clase derivate
};
