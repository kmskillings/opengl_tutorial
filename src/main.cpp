#define GLEW_NO_GLU
#define GLEW_STATIC
#include <glew.h>
#include <glfw3.h>

#include "scene.hpp"

int main(void)
{

    // Setup code
    GLFWwindow* window;

    Scene* scene;

    // Game loop
    while(glfwWindowShouldClose(window) == GL_FALSE) {

        // Draw the scene
        scene->draw();

        // Handle input
        glfwPollEvents();
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window, GL_TRUE);
        }

    }

}