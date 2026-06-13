#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define GLEW_NO_GLU
#define GLEW_STATIC
#include <glew.h>
#include <glfw3.h>
#include <cglm/cglm.h>

#include "textures/textures.h"
#include "shaders/shaders.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define WINDOW_TITLE "OpenGL Tutorial"

#define ROTATIONS_PER_SECOND 0.2f

#define MY_PI_F 3.1415927

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
        -1.0f, -1.0f, -1.0f,  1.0f,      0.0f, -1.0f,
         1.0f, -1.0f, -1.0f,  1.0f,      1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,  1.0f,     -1.0f,  0.0f,
        -1.0f, -1.0f,  1.0f,  1.0f,      0.0f,  0.0f,
         1.0f, -1.0f,  1.0f,  1.0f,      1.0f,  0.0f,
         1.0f, -1.0f, -1.0f,  1.0f,      2.0f,  0.0f,
        -1.0f, -1.0f, -1.0f,  1.0f,      3.0f,  0.0f,
        -1.0f,  1.0f, -1.0f,  1.0f,     -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,  1.0f,      0.0f,  1.0f,
         1.0f,  1.0f,  1.0f,  1.0f,      1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,  1.0f,      2.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,  1.0f,      3.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,  1.0f,      0.0f,  2.0f,
         1.0f,  1.0f, -1.0f,  1.0f,      1.0f,  2.0f
    };
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexArray), vertexArray, GL_STATIC_DRAW);

    GLuint elementArray[] = {
        0, 1, 4,
        4, 3, 0,
        2, 3, 8,
        8, 7, 2,
        3, 4, 9,
        9, 8, 3,
        4, 5, 10,
        10, 9, 4,
        5, 6, 11,
        11, 10, 5,
        8, 9, 13,
        13, 12, 8
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

    mat4 transformModel;

    mat4 transformView;
    vec3 positionCamera = {0.0f, 2.0f, 5.0f};
    vec3 positionLookAt = {0.0f, 0.0f, 0.0f};
    vec3 directionUp = {0.0f, 1.0f, 0.0f};
    glm_lookat(positionCamera, positionLookAt, directionUp, transformView);

    mat4 transformProjection;
    glm_perspective(
        MY_PI_F / 4.0,
        (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT,
        0.1f, 10.0f,
        transformProjection
    );

    mat4 transformTotal;
    GLuint uniformTransform = glGetUniformLocation(shaderProgram, "transform");

    GLuint camiTexture;
    glGenTextures(1, &camiTexture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, camiTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_BGRA, CAMI_TEXTURE_WIDTH, CAMI_TEXTURE_HEIGHT, 0, GL_BGRA, GL_UNSIGNED_BYTE, camiTextureBytes);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    GLuint camiTextureUniform = glGetUniformLocation(shaderProgram, "textureCami");
    glUniform1i(camiTextureUniform, 0);

    struct timespec ts;
    timespec_get(&ts, TIME_UTC);
    int64_t timeStart = ts.tv_sec * 1000 + (ts.tv_nsec / 1000000);

    float radiansPerMillisecond = MY_PI_F * ROTATIONS_PER_SECOND / 1000;

    glEnable(GL_DEPTH_TEST);

    while(glfwWindowShouldClose(window) == GL_FALSE)
    {
        timespec_get(&ts, TIME_UTC);
        int64_t timeNow = ts.tv_sec * 1000 + (ts.tv_nsec / 1000000);
        int timeSinceStart = timeStart - timeNow;

        float rotationAngle = radiansPerMillisecond * timeSinceStart;
        vec3 rotationAxis = {0.7f, 0.5f, 0.2f};
        glm_rotate_make(transformModel, rotationAngle, rotationAxis);
        glm_mat4_mul(transformProjection, transformView, transformTotal);
        glm_mat4_mul(transformTotal, transformModel, transformTotal);
        glUniformMatrix4fv(uniformTransform, 1, GL_FALSE, &transformTotal[0][0]);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glDrawElements(GL_TRIANGLES, sizeof(elementArray) / sizeof(elementArray[0]), GL_UNSIGNED_INT, 0);

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