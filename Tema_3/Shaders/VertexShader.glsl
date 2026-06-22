#version 330 // Versiunea GLSL

layout(location = 0) in vec3 v_position; // Pozitia vertexului
layout(location = 1) in vec3 v_normal; // Normala vertexului
layout(location = 2) in vec2 v_texture_coord; // Coordonate UV

uniform mat4 Model; // Matricea de model
uniform mat4 View; // Matricea de vizualizare
uniform mat4 Projection; // Matricea de proiectie
uniform float time; // Timpul global
uniform int tree_part; // 0: non-tree, 1: trunk, 2: leaves
uniform float tree_height; // Inaltimea copacului
uniform float bend_strength; // Intensitatea vantului
uniform float bend_phase; // Faza pentru vant

out vec2 texcoord; // UV spre fragment shader
out vec3 world_position; // Pozitie in world space
out vec3 world_normal; // Normala in world space

void main() // Intrarea in shader
{ // Deschidere main
    vec3 pos = v_position; // Copie pozitie pentru deformare

    if (tree_part != 0) { // Daca este copac
        float t = 0.0; // Procent din inaltime
        if (tree_height > 0.0001) { // Evita impartirea la zero
            t = clamp(pos.y / tree_height, 0.0, 1.0); // Normalizare pe inaltime
        }

        if (tree_part == 1) { // Daca e trunchi
            // Subtiere trunchi pe inaltime
            float scale = 1.0 - t; // Factor de subtiere
            pos.x *= scale; // Subtiere pe X
            pos.z *= scale; // Subtiere pe Z
        } else if (tree_part == 2) { // Daca e frunzis
            // Vant pe frunzis
            float wave = sin(time * 1.5 + t * 0.5 + bend_phase); // Unda in timp
            float offset = wave * bend_strength * t; // Deplasare orizontala
            pos.x += offset; // Aplica deplasarea
        } // Sfarsit frunzis
    } // Sfarsit copac

    texcoord = v_texture_coord; // Trimite UV
    world_position = (Model * vec4(pos, 1.0)).xyz; // Calcul pozitie world
    world_normal = normalize(mat3(Model) * v_normal); // Transformare normala

    gl_Position = Projection * View * Model * vec4(pos, 1.0); // Pozitia finala in clip space
}