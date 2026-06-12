#include <stdlib.h>
#include <stdio.h>

#define GLEW_NO_GLU
#define GLEW_STATIC
#include <glew.h>

#include <glfw3.h>

#include "textures/textures.h"
#include "shaders/shaders.h"

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

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    float vertexArray[] = {
        // XYZW coords                  Texture coords
        -1.0f, -1.0f,  0.0f,  1.0f,     0.0f,  0.0f,        // Lower-left corner
         1.0f, -1.0f,  0.0f,  1.0f,     1.0f,  0.0f,        // Lower-right corner
        -1.0f,  1.0f,  0.0f,  1.0f,     0.0f,  1.0f,        // Upper-left corner
         1.0f,  1.0f,  0.0f,  1.0f,     1.0f,  1.0f         // Upper-right corner
    };
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexArray), vertexArray, GL_STATIC_DRAW);

    GLuint elementArray[] = {
        0, 1, 2,                // Lower-left triangle
        3, 2, 1                 // Upper-right triangle
    };
    GLuint ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elementArray), elementArray, GL_STATIC_DRAW);

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glBindFragDataLocation(shaderProgram, 0, "outColor");
    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);

    GLuint posAttrib = glGetAttribLocation(shaderProgram, "position");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 4, GL_FLOAT, GL_FALSE, 6*sizeof(float), 0);

    GLuint texCoordAttrib = glGetAttribLocation(shaderProgram, "texCoordVert");
    glEnableVertexAttribArray(texCoordAttrib);
    glVertexAttribPointer(texCoordAttrib, 2, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(4*sizeof(float)));

    GLuint camiTexture;
    glGenTextures(1, &camiTexture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, camiTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_BGRA, CAMI_TEXTURE_WIDTH, CAMI_TEXTURE_HEIGHT, 0, GL_BGRA, GL_UNSIGNED_BYTE, camiTextureBytes);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    GLuint camiTextureUniform = glGetUniformLocation(shaderProgram, "textureCami");
    glUniform1i(camiTextureUniform, 0);

    printf("Ready\n");

    while(glfwWindowShouldClose(window) == GL_FALSE)
    {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window, GL_TRUE);
        }
    }

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