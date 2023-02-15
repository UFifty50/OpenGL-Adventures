#include <stdio.h>
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"


GLFWwindow* window(int width, int height, const char* title, GLFWmonitor* monitor, GLFWwindow* share) {
    if (!::glfwInit()) {
        ::fprintf(stderr, "Failed to initialize GLFW\n");
        return nullptr;
    }

    ::glfwWindowHint(GLFW_SAMPLES, 4);
    ::glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    ::glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    ::glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    ::glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window;
    window = ::glfwCreateWindow(width, height, title, monitor, share);
    if (window == nullptr) {
        ::fprintf(stderr, "Failed to open GLFW window.\n");
        ::glfwTerminate();
        return nullptr;
    }

    ::glfwMakeContextCurrent(window);
    ::glewExperimental = true;
    ::glfwSwapInterval(0);
    if (::glewInit() != GLEW_OK) {
        ::fprintf(stderr, "Failed to initialize GLEW.");
        ::glfwTerminate();
        return nullptr;
    }

    return window;
}