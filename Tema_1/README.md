# Tema 1 â€” Mini-jocuri 2D OpenGL

Proiect **standalone** (propriul `MainLauncher.cpp`), separat de laboratoarele gfx-framework.

## Jocuri incluse

| TastÄƒ | ScenÄƒ |
|-------|--------|
| F1 / 1 | Editor navÄƒ |
| F2 / 2 | Breakout |
| F3 / 3 | Chicken Invaders |
| F4 / 4 | Pong |

## StructurÄƒ

- `MainLauncher.cpp` â€” punct de intrare, comutare scene
- `src/` â€” implementÄƒri jocuri
- `include/` â€” headere
- `shaders/` â€” vertex/fragment GLSL

## Compilare (Linux / WSL / MinGW)

```bash
g++ -std=c++17 MainLauncher.cpp src/*.cpp -Iinclude -lglfw -lGL -lGLEW -lGLU -lglut -ldl -o Tema1Launcher
```
