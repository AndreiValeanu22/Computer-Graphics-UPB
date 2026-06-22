# Tema 1 — Mini-jocuri 2D OpenGL

Proiect **standalone** (cu propriul `MainLauncher.cpp`), separat de laboratoarele gfx-framework.

## Jocuri incluse

| Tastă | Scenă |
|-------|--------|
| F1 / 1 | Editor navă |
| F2 / 2 | Breakout |
| F3 / 3 | Chicken Invaders |
| F4 / 4 | Pong |

## Structură

- `MainLauncher.cpp` — punct de intrare, comutarea scenelor
- `src/` — implementările jocurilor
- `include/` — headere
- `shaders/` — vertex/fragment GLSL

## Compilare (Linux / WSL / MinGW)

```bash
g++ -std=c++17 MainLauncher.cpp src/*.cpp -Iinclude -lglfw -lGL -lGLEW -lGLU -lglut -ldl -o Tema1Launcher
```
