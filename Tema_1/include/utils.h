#pragma once
#include <GL/glew.h>
#include <string>

// Functie care incarca un shader OpenGL din doua fisiere sursa
unsigned int LoadShader(const std::string& vertexPath, const std::string& fragmentPath); // returneaza id-ul GPU al shaderului incarcat