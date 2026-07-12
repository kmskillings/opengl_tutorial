#define GLEW_NO_GLU
#define GLEW_STATIC
#include <glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <cmath>

extern "C" {
#include "textures.h"
#include "shaders.h"
}

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define WINDOW_TITLE "Cami Cube"

#define CLOUD_RADIUS 10.0f
#define CLOUD_ROTATION_RATE_MAX 1.0f

#define CAMERA_SPEED_TRANSLATION 5.0f
#define CAMERA_SENSITIVITY_PITCH 0.1f
#define CAMERA_SENSITIVITY_YAW   0.1f

#define THROB_PERIOD_MILLIS 2000

GLFWwindow* setupGl(void);
double secondsSinceEpoch(void);
void reportShaderStatus(GLuint shader);

int main(void)
{

    GLuint error;

    // Setup code
    GLFWwindow* window = setupGl();
    glfwMakeContextCurrent(window);
    glewInit();
    error = glGetError();

    // Load Cami texture
    GLuint textureCami;
    glGenTextures(1, &textureCami);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureCami);
    glTexImage2D(
        GL_TEXTURE_2D, 
        0, 
        GL_BGRA, 
        CAMI_TEXTURE_WIDTH, 
        CAMI_TEXTURE_HEIGHT, 
        0, 
        GL_BGRA, 
        GL_UNSIGNED_BYTE, 
        camiTextureBytes
    );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    error = glGetError();

    // Load Cube mesh
    GLuint vaoCube;
    GLuint vboCube;
    GLuint eboCube;
    glGenVertexArrays(1, &vaoCube);
    glBindVertexArray(vaoCube);
    glGenBuffers(1, &vboCube);
    glBindBuffer(GL_ARRAY_BUFFER, vboCube);
    float cubeVertices[] = 
    {
        -1.0f, -1.0f, -1.0f,    0.0f, -1.0f,
         1.0f, -1.0f, -1.0f,    1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,   -1.0f,  0.0f,
        -1.0f, -1.0f,  1.0f,    0.0f,  0.0f,
         1.0f, -1.0f,  1.0f,    1.0f,  0.0f,
         1.0f, -1.0f, -1.0f,    2.0f,  0.0f,
        -1.0f, -1.0f, -1.0f,    3.0f,  0.0f,
        -1.0f,  1.0f, -1.0f,   -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,    0.0f,  1.0f,
         1.0f,  1.0f,  1.0f,    1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,    2.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,    3.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,    0.0f,  2.0f,
         1.0f,  1.0f, -1.0f,    1.0f,  2.0f
    }; // Side length is hard-coded as 2 because we can always adjust it later
       // in the model matrix.
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(cubeVertices) / sizeof(cubeVertices[0]),
        cubeVertices,
        GL_STATIC_DRAW
    );
    glVertexAttribPointer(  // 3D world position in location 0
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        5 * sizeof(float),
        (void*)0
    );
    glVertexAttribPointer(  // 2D texture coordinate in location 1
        1,
        2,
        GL_FLOAT,
        GL_FALSE,
        5 * sizeof(float),
        (void*)(3 * sizeof(float))
    );
    glGenBuffers(1, &eboCube);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboCube);
    uint cubeElements[] = {
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
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        sizeof(cubeElements) / sizeof(cubeElements[0]),
        cubeElements,
        GL_STATIC_DRAW
    );
    glBindVertexArray(0);
    error = glGetError();

    // Load and configure shader
    GLuint shaderVertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(shaderVertex, 1, &shaderVertexSource, NULL);
    glCompileShader(shaderVertex);
    reportCompileStatus(shaderVertex);
    GLuint shaderFragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(shaderFragment, 1, &shaderFragmentSource, NULL);
    glCompileShader(shaderFragment);
    reportCompileStatus(shaderFragment);
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, shaderVertex);
    glAttachShader(shaderProgram, shaderFragment);
    glLinkProgram(shaderProgram);
    reportLinkStatus(shaderProgram);
    error = glGetError();
    
    // Create array of model matrices
    float dataTransform[] = {
        #include "cloud.txt"
    };
    constexpr uint countTransform = sizeof(dataTransform) / sizeof(float) / 11;
    glm::mat4 matricesModel[countTransform];
    for (int i = 0; i < countTransform; i++)
    {
        glm::vec3 position = glm::vec3(
            dataTransform[11 * i + 0],
            dataTransform[11 * i + 1],
            dataTransform[11 * i + 2]
        );
        float angle = dataTransform[11 * i + 3];
        glm::vec3 axis = glm::vec3(
            dataTransform[11 * i + 4],
            dataTransform[11 * i + 5],
            dataTransform[11 * i + 6]
        );
        glm::vec3 scale = glm::vec3(1.0f);
        glm::mat4 matrix = glm::identity<glm::mat4>();
        matrix = glm::translate(matrix, position);
        matrix = glm::rotate(matrix, angle, axis);
        matrix = glm::scale(matrix, scale);
        matricesModel[i] = matrix;
    }

    // Just leave the camera at the origin for now, looking straight ahead.
    glm::vec3 positionCamera = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::quat orientationCamera = glm::angleAxis(
        0.0f,
        glm::vec3(1.0f, 0.0f, 0.0f)
    );

    // Calculate project matrix
    glm::mat4 matrixProject = glm::perspective(
        (float)(M_PI / 4.0f),
        (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT,
        0.1f,
        100.0f
    );

    // Set up for the render cycle

    // Activate the cube mesh
    glBindVertexArray(vaoCube);

    // Activate the shader program
    glUseProgram(shaderProgram);
    glUniform1i(1, 0);  // Diffuse texture is in location 1. Only texture
                        // unit 0 is used.
    error = glGetError();

    while(glfwWindowShouldClose(window) == GL_FALSE) {

        glfwPollEvents();
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window, GL_TRUE);
        }

        // Calculate the view matrix
        glm::mat4 matrixView = glm::identity<glm::mat4>();
        matrixView = glm::mat4_cast(glm::inverse(orientationCamera)) * matrixView;
        matrixView = glm::translate(matrixView, -positionCamera);
        // Rotation is applied before translation because the view matrix is
        // "backwards."

        glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);


        for (int i = 0; i < countTransform; i++)
        {
            // Calculate the aggregate matrix
            glm::mat4 matrix = matrixProject * matrixView * matricesModel[i];
            glUniformMatrix4fv(
                0,
                1,
                GL_FALSE,
                glm::value_ptr(matrix)
            );

            glDrawElements(
                GL_TRIANGLES,
                sizeof(cubeElements) / sizeof(cubeElements[0]),
                GL_UNSIGNED_INT,
                0
            );
            error = glGetError();
        }

        glfwSwapBuffers(window);
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

