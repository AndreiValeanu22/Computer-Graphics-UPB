// /Tema1/include/gl_includes.h
#pragma once

// Daca legi GLEW static, definesti GLEW_STATIC (fie aici, fie prin intermediul flag-ului compilatorului):
// #define GLEW_STATIC
#include <GL/glew.h>
// --- GLEW trebuie sa fie declarat primul OpenGL header


// --- Windows primul: face APIENTRY/WINGDIAPI valabil and evita ciocnirile macro (macro clashes)
#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#endif


// --- Windowing / input (dupa GLEW)
#include <GLFW/glfw3.h>
// --- GLEW TREBUIE sa fie primul antet OpenGL


// --- Optional (sigur dupa GLEW)
#if defined(__APPLE__)
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif



// --- Text / bitmap (GLUT sau freeglut) (dupa GLEW; GLUT extrage intern gl.h)
#if defined(__APPLE__)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
