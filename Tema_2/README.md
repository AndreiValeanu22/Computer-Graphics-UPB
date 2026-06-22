# Tema 2 — Train Geeks

Modul pentru **gfx-framework** (`m1::Tema2`).

## Integrare în framework

1. Copiază conținutul în `gfx-framework/src/lab_m1/Tema2/`
2. În `lab_list.h`: `#include "lab_m1/Tema2/tema2.h"`
3. În `main.cpp`: `World* world = new m1::Tema2();`
4. Compilează și rulează `GFXFramework`

## Funcționalități implementate

- Teren colorat în funcție de tip (Field / River / Mountain)
- Plasarea și editarea șinelor (Normal / Bridge / Tunnel)
- Tren animat pe șine, stații, shadere Terrain + GhostTrail
