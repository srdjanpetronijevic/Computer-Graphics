#pragma once

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

unsigned int loadImageToTexture(const char* filePath);
unsigned int loadAndSetupTexture(const char* filePath);
