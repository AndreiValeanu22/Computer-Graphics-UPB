#version 330

layout(location = 0) in vec3 position; // Preia coordonatele X,Y,Z ale fiecarui punct din obiect.
layout(location = 1) in vec4 color; // Preia culoarea definita pentru fiecare punct in CPU.

uniform mat4 Model; // Matricea care pune obiectul la coordonatele cursorului în lume.
uniform mat4 View; // Matricea care transforma lumea in functie de pozitia camerei.
uniform mat4 Projection; // Matricea care creeaza efectul de perspectiva (obiectele departate sunt mai mici).

out vec4 vColor; // Variabila de iesire care trimite culoarea catre fragment shader pentru colorare.

void main() {
    vColor = color; // Paseaza culoarea primita mai departe fara modificari.
    gl_Position = Projection * View * Model * vec4(position, 1.0); // Calculeaza pozitia finala combinand cele 3 matrice; ajuta la afisarea corecta in spatiu.
}