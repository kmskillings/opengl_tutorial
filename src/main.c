#include <stdlib.h>
#include <stdio.h>

#define GLEW_NO_GLU
#define GLEW_STATIC
#include <glew.h>

#include <glfw3.h>

#include "textures/textures.h"
#include "shaders/shaders.h"

#include "model.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define WINDOW_TITLE "OpenGL Tutorial"

GLFWwindow* setupGlfw(void);
void        setupGlew(void);
void        checkShaderCompilation(GLuint shader);

int main(void)
{
    GLFWwindow* window = setupGlfw();
    setupGlew();

    initializeTextures();

    glfwTerminate();

    return EXIT_SUCCESS;
}

GLFWwindow* setupGlfw(void) {

    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, NULL, NULL);

    glfwMakeContextCurrent(window);

    return window;
}

void setupGlew(void) {
    glewExperimental = GL_TRUE;
    glewInit();
}

void checkShaderCompilation(GLuint shader) {
    GLuint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    char message[512];
    glGetShaderInfoLog(shader, 512, NULL, message);
    printf("Shader message: %s\n", message);
}