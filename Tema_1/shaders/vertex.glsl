#version 330 core // versiunea GLSL
layout(location = 0) in vec3 aPos; // primeste pozitia varfului din mesh
void main() { gl_Position = vec4(aPos,1.0); } // proiecteaza varful final transform