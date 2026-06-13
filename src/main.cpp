#define GLEW_NO_GLU
#define GLEW_STATIC
#include <glew.h>
#include <glfw3.h>

#include "material.hpp"
#include "mesh.hpp"
#include "model.hpp"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define WINDOW_TITLE "Cami Cube"

GLFWwindow* setupGl(void);

int main(void)
{

    // Setup code
    GLFWwindow* window = setupGl();
    glfwMakeContextCurrent(window);
    glewInit();

    glm::vec4 color;
    color.r = 1.0f;
    color.g = 0.0f;
    color.b = 0.0f;
    color.a = 1.0f;

    GlWorld::Model* model = new GlWorld::Model(color);
    GLuint error = glGetError();

    // Game loop
    while(glfwWindowShouldClose(window) == GL_FALSE) {

        // Draw the scene
        model->drawRgb();
        GLuint error = glGetError();
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

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    return glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, NULL, NULL);

}