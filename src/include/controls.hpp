#ifndef CONTROLS_HPP
#define CONTROLS_HPP

#include <glm/glm.hpp>

namespace controls {
    double DeltaTime();
    void computeMatricesFromInputs(GLFWwindow* window, float deltaTime);
    glm::mat4 getViewMatrix();
    glm::mat4 getProjectionMatrix();

    void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
}

#endif
