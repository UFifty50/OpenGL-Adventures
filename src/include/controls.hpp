#ifndef CONTROLS_HPP
#define CONTROLS_HPP

#include <glm/glm.hpp>


void computeMatricesFromInputs(GLFWwindow* window);
glm::mat4 getViewMatrix();
glm::mat4 getProjectionMatrix();

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

#endif
