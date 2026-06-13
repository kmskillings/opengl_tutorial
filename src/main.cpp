#define GLEW_NO_GLU
#define GLEW_STATIC
#include <glew.h>
#include <glfw3.h>

#include "scene.hpp"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define WINDOW_TITLE "Cami Cube"

GLFWwindow* setupGl(void);

int main(void)
{

    // Setup code
    GLFWwindow* window = setupGl();
    glfwMakeContextCurrent(window);

    glm::vec4 skyColor = glm::vec4(0.5f, 0.9f, 1.0f, 1.0f);
    GlWorld::Scene* scene = new GlWorld::Scene(skyColor);

    // Game loop
    while(glfwWindowShouldClose(window) == GL_FALSE) {

        // Draw the scene
        scene->draw();
        glfwSwapBuffers(window);

        // Handle input
        glfwPollEvents();
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window, GL_TRUE);
        }

    }

}

GLFWwindow* setupGl(void) {

    glewInit();
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    return glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, NULL, NULL);

}