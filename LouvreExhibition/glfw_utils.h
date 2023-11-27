#pragma once
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

GLFWwindow* initializeGLFW(unsigned int width, unsigned int height, const char* title);
bool initializeGLEW();