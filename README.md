# Computer-Graphics-UPB â€” Elemente de GraficÄƒ pe Calculator (UPB ACS)

Proiecte personale pentru cursul **EGC** (Elemente de GraficÄƒ pe Calculator), Anul 3 **CB**, an universitar **2025â€“2026**.

| Folder | TemÄƒ oficialÄƒ | Tehnologii |
|--------|---------------|------------|
| [**Tema_1**](Tema_1/) | Mini-jocuri 2D + launcher | C++, OpenGL, GLFW, GLEW, GLUT |
| [**Tema_2**](Tema_2/) | **Train Geeks** | C++, OpenGL 3D, gfx-framework UPB |
| [**Tema_3**](Tema_3/) | **Firefly Forest** (temÄƒ suplimentarÄƒ) | C++, shadere GLSL, gfx-framework UPB |

---

## Tema 1 â€” Launcher mini-jocuri 2D (standalone)

**Ce am implementat:**
- **Editor navÄƒ** â€” construieÈ™ti nava din componente; o poÈ›i trimite Ã®n Chicken Invaders
- **Breakout** â€” paletÄƒ, minge, cÄƒrÄƒmizi, particule
- **Chicken Invaders** â€” navÄƒ personalizabilÄƒ, inamici, proiectile
- **Pong** â€” 2 jucÄƒtori, coliziuni

**Rulare** (proiect independent, **nu** foloseÈ™te gfx-framework):

```bash
cd Tema_1
g++ -std=c++17 MainLauncher.cpp src/*.cpp -Iinclude -lglfw -lGL -lGLEW -lGLU -lglut -ldl -o Tema1Launcher
./Tema1Launcher
```

**Controale launcher:** F1 Editor Â· F2 Breakout Â· F3 Chicken Invaders Â· F4 Pong Â· ESC ieÈ™ire

---

## Tema 2 â€” Train Geeks (gfx-framework)

**Ce am implementat:**
- HartÄƒ pe grilÄƒ cu relief: cÃ¢mpie (verde), rÃ¢u (albastru), munte (maro)
- È˜ine: drepte, colÈ›uri, pod, tunel; unelte de editare pe grilÄƒ
- Tren (locomotivÄƒ + vagoane) construit din primitive 3D; miÈ™care automatÄƒ pe È™ine cu interpolare
- GÄƒri/staÈ›ii; shadere custom: **Terrain**, **GhostTrail**
- FiÈ™iere: `tema2.cpp`, `tema2.h`, `object3D.cpp`, shadere Ã®n `shaders/`

**Rulare:** copiazÄƒ folderul Ã®n `src/lab_m1/Tema2/` din [gfx-framework](https://github.com/UPB-Graphics-Framework/gfx-framework), decomenteazÄƒ `#include` Ã®n `lab_list.h` È™i `World* world = new m1::Tema2();` Ã®n `main.cpp`, apoi build `GFXFramework`.

**Termen curs:** 14 decembrie 2025, 23:59 (BucureÈ™ti) Â· ~3 sÄƒptÄƒmÃ¢ni

Detalii: [Tema_2/README.md](Tema_2/README.md)

---

## Tema 3 â€” Firefly Forest (gfx-framework, suplimentar)

**Ce am implementat:**
- PÄƒdure nocturnÄƒ: teren texturat, copaci (trunchi cilindric + frunziÈ™ pe nivele)
- **Licurici** emisivi pe orbite; **turn de observaÈ›ie** (stalpi, platformÄƒ, acoperiÈ™ con)
- **CeaÈ›Äƒ** exponenÈ›ialÄƒ; **light cookies** pe spotlights
- Shadere custom: animaÈ›ie vÃ¢nt Ã®n vertex shader (`Shaders/VertexShader.glsl`, `FragmentShader.glsl`)

**Rulare:** la fel ca Tema 2, dar scena `m1::Tema3` Ã®n `main.cpp`. NecesitÄƒ texturi Ã®n `Tema_3/textures/` (`bark.jpg`, `leaves.jpg`, `wood_planks.png`, `grass.jpeg`, `wood.jpg`) â€” pe repo sunt incluse doar `cone.obj` È™i `cylinder.obj`; restul texturilor le adaugi local din materialele cursului.

**Termen curs:** 18 ianuarie 2026, 23:59 (BucureÈ™ti) Â· temÄƒ suplimentarÄƒ Â· ~3 sÄƒptÄƒmÃ¢ni

Detalii: [Tema_3/README.md](Tema_3/README.md)

---

## Ce nu este pe GitHub

- Arhive `.zip` / `.rar`
- Folderul complet **gfx-framework** (motorul cursului â€” doar sursele mele Tema 2/3)
- Build-uri Visual Studio (`build/`, `.vs/`), executabile
- `IMPORTANT.txt` È™i note personale locale

---

## LicenÈ›Äƒ

Cod propriu â€” teme academice **UPB ACS / EGC**. PoÈ›i folosi repo-ul ca portofoliu; menÈ›ioneazÄƒ cursul dacÄƒ refoloseÈ™ti structura.
