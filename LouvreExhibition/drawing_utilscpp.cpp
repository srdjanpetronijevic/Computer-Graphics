#include "drawing_utils.h"

void drawRotatedSquare(int index, float radius, float rotationSpeed, int uPosLoc) {
    glUniform2f(uPosLoc, radius * cos(glfwGetTime() * rotationSpeed), radius * sin(glfwGetTime() * rotationSpeed));
    glDrawArrays(GL_TRIANGLE_STRIP, index * 4, 4);
}

