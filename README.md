# Computer-Graphics-UPB — Elemente de Grafică pe Calculator (UPB ACS)

Proiecte personale pentru cursul **EGC** (Elemente de Grafică pe Calculator), Anul 3 **CB**, an universitar **2025–2026**.

| Folder | Temă oficială | Tehnologii |
|--------|---------------|------------|
| [**Tema_1**](Tema_1/) | Mini-jocuri 2D + launcher | C++, OpenGL, GLFW, GLEW, GLUT |
| [**Tema_2**](Tema_2/) | **Train Geeks** | C++, OpenGL 3D, gfx-framework UPB |
| [**Tema_3**](Tema_3/) | **Firefly Forest** (temă suplimentară) | C++, shadere GLSL, gfx-framework UPB |

---

## Tema 1 — Launcher mini-jocuri 2D (standalone)

**Ce am implementat:**
- **Editor navă** — construiești nava din componente; o poți trimite în Chicken Invaders
- **Breakout** — paletă, minge, cărămizi, particule
- **Chicken Invaders** — navă personalizabilă, inamici, proiectile
- **Pong** — 2 jucători, coliziuni

**Rulare** (proiect independent, **nu** folosește gfx-framework):

```bash
cd Tema_1
g++ -std=c++17 MainLauncher.cpp src/*.cpp -Iinclude -lglfw -lGL -lGLEW -lGLU -lglut -ldl -o Tema1Launcher
./Tema1Launcher
```

**Controale launcher:** F1 Editor · F2 Breakout · F3 Chicken Invaders · F4 Pong · ESC ieșire

---

## Tema 2 — Train Geeks (gfx-framework)

**Ce am implementat:**
- Hartă pe grilă cu relief: câmpie (verde), râu (albastru), munte (maro)
- Șine: drepte, colțuri, pod, tunel; unelte de editare pe grilă
- Tren (locomotivă + vagoane) construit din primitive 3D; mișcare automată pe șine cu interpolare
- Gări/stații; shadere custom: **Terrain**, **GhostTrail**
- Fișiere: `tema2.cpp`, `tema2.h`, `object3D.cpp`, shadere în `shaders/`

**Rulare:** copiază folderul în `src/lab_m1/Tema2/` din [gfx-framework](https://github.com/UPB-Graphics-Framework/gfx-framework), decomentează `#include` în `lab_list.h` și `World* world = new m1::Tema2();` în `main.cpp`, apoi compilează `GFXFramework`.

**Termen curs:** 14 decembrie 2025, 23:59 (București) · ~3 săptămâni

Detalii: [Tema_2/README.md](Tema_2/README.md)

---

## Tema 3 — Firefly Forest (gfx-framework, suplimentar)

**Ce am implementat:**
- Pădure nocturnă: teren texturat, copaci (trunchi cilindric + frunziș pe nivele)
- **Licurici** emisivi pe orbite; **turn de observație** (stâlpi, platformă, acoperiș conic)
- **Ceață** exponențială; **light cookies** pe spotlights
- Shadere custom: animație de vânt în vertex shader (`Shaders/VertexShader.glsl`, `FragmentShader.glsl`)

**Rulare:** la fel ca Tema 2, dar cu scena `m1::Tema3` în `main.cpp`. Texturile sunt incluse în `Tema_3/textures/` (`bark.jpg`, `leaves.jpg`, `wood_planks.png`, `grass.jpeg`, `wood.jpg`). Modelele `cone.obj` și `cylinder.obj` trebuie adăugate local (sunt furnizate de framework).

**Termen curs:** 18 ianuarie 2026, 23:59 (București) · temă suplimentară · ~3 săptămâni

Detalii: [Tema_3/README.md](Tema_3/README.md)

---

## Ce nu este pe GitHub

- Arhive `.zip` / `.rar`
- Folderul complet **gfx-framework** (motorul cursului — doar sursele mele pentru Tema 2/3)
- Build-uri Visual Studio (`build/`, `.vs/`), executabile
- `IMPORTANT.txt` și note personale locale

---

## Licență

Cod propriu — teme academice **UPB ACS / EGC**. Poți folosi repo-ul ca portofoliu; menționează cursul dacă refolosești structura.
