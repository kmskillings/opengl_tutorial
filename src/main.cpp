#define GLEW_NO_GLU
#define GLEW_STATIC
#include <glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <cmath>
#include <chrono>

extern "C" {
#include "textures.h"
#include "shaders.h"
}

#include "transform.hpp"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define WINDOW_TITLE "Cami Cube"

#define CLOUD_RADIUS 10.0f
#define CLOUD_ROTATION_RATE_MAX 1.0f

#define CAMERA_SPEED_TRANSLATION 5.0f
#define CAMERA_SENSITIVITY_PITCH 0.1f
#define CAMERA_SENSITIVITY_YAW   0.1f
#define CAMERA_SPEED_ROLL        1.0f

#define THROB_PERIOD_MILLIS 2000

GLFWwindow* setupGl(void);
double secondsSinceEpoch(void);

// A Cami Cube consists of a Transform and an angle and axis representing its
// incremental rotation. It exposes a method to tick its rotation and one to
// get its model matrix.
class CamiCube
{

private:
    Transform transform_;
    float rate_;
    glm::vec3 axis_;

public:

    CamiCube(void) {}

    CamiCube(
        const glm::vec3& position,
        const float& orientationAngle,
        const glm::vec3& orientationAxis,
        const float& rotationRate,
        const glm::vec3& rotationAxis,
        const glm::vec3& scale
    ) :
        transform_(Transform(
            position,
            orientationAngle,
            orientationAxis,
            scale
        )),
        rate_(rotationRate),
        axis_(rotationAxis)
    {}

    void update(const float& secondsDelta)
    {
        transform_.rotate(
            rate_ * secondsDelta,
            axis_
        );
    }

    glm::mat4 getMatrix(void)
    {
        return transform_.getMatrix();
    }

};

class Lightbulb
{

private:
    glm::vec3 center;

public:

    Lightbulb(void);

};

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
        sizeof(cubeVertices),
        cubeVertices,
        GL_STATIC_DRAW
    );
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(  // 3D world position in location 0
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        5 * sizeof(float),
        (void*)0
    );
    glEnableVertexAttribArray(1);
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
        sizeof(cubeElements),
        cubeElements,
        GL_STATIC_DRAW
    );
    glBindVertexArray(0);
    error = glGetError();

    // Load and configure shader
    GLuint shaderProgram = compileShader(
        &shaderVertexSource, 1,
        &shaderFragmentSource, 1
    );
    
    // Create array of model matrices
    float dataTransform[] = {
        #include "cloud.txt"
    };
    constexpr uint countTransform = sizeof(dataTransform) / sizeof(float) / 11;
    CamiCube camiCubes[countTransform];
    for (int i = 0; i < countTransform; i++)
    {
        glm::vec3 position = glm::vec3(
            dataTransform[11 * i + 0],
            dataTransform[11 * i + 1],
            dataTransform[11 * i + 2]
        );
        position = position * CLOUD_RADIUS;
        float orientationAngle = dataTransform[11 * i + 3];
        glm::vec3 orientationAxis = glm::vec3(
            dataTransform[11 * i + 4],
            dataTransform[11 * i + 5],
            dataTransform[11 * i + 6]
        );
        float rotationRate = dataTransform[11 * i + 7] * CLOUD_ROTATION_RATE_MAX;
        glm::vec3 rotationAxis = glm::vec3(
            dataTransform[11 * i + 8],
            dataTransform[11 * i + 9],
            dataTransform[11 * i + 10]
        );
        glm::vec3 scale = glm::vec3(0.5f);
        camiCubes[i] = CamiCube(
            position,
            orientationAngle,
            orientationAxis,
            rotationRate,
            rotationAxis,
            scale
        );
    }

    // Just leave the camera at the origin for now, looking straight ahead.
    glm::vec3 positionCamera = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::quat orientationCamera = glm::angleAxis(
        0.0f,
        glm::vec3(1.0f, 0.0f, 0.0f)
    );
    Transform cameraTransform = Transform(
        positionCamera, 
        orientationCamera, 
        glm::vec3(1.0f)
    );

    // Calculate project matrix
    glm::mat4 matrixProject = glm::perspective(
        (float)(M_PI / 3.0f),
        (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT,
        0.1f,
        100.0f
    );

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Set up for the render cycle

    // Activate the cube mesh
    glBindVertexArray(vaoCube);

    // Activate the shader program
    glUseProgram(shaderProgram);
    glUniform1i(1, 0);  // Diffuse texture is in location 1. Only texture
                        // unit 0 is used.
    glEnable(GL_DEPTH_TEST);
    error = glGetError();

    double mouseXNow;
    double mouseYNow;
    double mouseXLast;
    double mouseYLast;
    double mouseXDelta;
    double mouseYDelta;
    glfwGetCursorPos(window, &mouseXNow, &mouseYNow);

    double secondsNow = secondsSinceEpoch();
    double secondsLast;
    double secondsDelta;

    while(glfwWindowShouldClose(window) == GL_FALSE) {

        secondsLast = secondsNow;
        secondsNow = secondsSinceEpoch();
        secondsDelta = secondsNow - secondsLast;

        glfwPollEvents();
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window, GL_TRUE);
        }

        // Handle camera motion
        glm::vec3 cameraDirection = glm::vec3(0.0f);
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        {
            cameraDirection = cameraDirection + 
                glm::vec3( 0.0f,  0.0f, -1.0f);
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        {
            cameraDirection = cameraDirection + 
                glm::vec3( 0.0f,  0.0f,  1.0f);
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        {
            cameraDirection = cameraDirection + 
                glm::vec3(-1.0f,  0.0f,  0.0f);
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        {
            cameraDirection = cameraDirection + 
                glm::vec3( 1.0f,  0.0f,  0.0f);
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        {
            cameraDirection = cameraDirection + 
                glm::vec3( 0.0f, -1.0f,  0.0f);
        }
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        {
            cameraDirection = cameraDirection + 
                glm::vec3( 0.0f,  1.0f,  0.0f);
        }
        if (glm::length(cameraDirection) > 0.1)
        {
            cameraDirection = glm::normalize(cameraDirection);
        }
        cameraTransform.translate(
            CAMERA_SPEED_TRANSLATION * secondsDelta,
            cameraDirection,
            Transform::Axes::Local
        );

        mouseXLast = mouseXNow;
        mouseYLast = mouseYNow;
        glfwGetCursorPos(window, &mouseXNow, &mouseYNow);
        mouseXDelta = mouseXNow - mouseXLast;
        mouseYDelta = mouseYNow - mouseYLast;
        cameraTransform.rotate(
            mouseYDelta * CAMERA_SENSITIVITY_PITCH * secondsDelta,
            glm::vec3(-1.0f, 0.0f, 0.0f),
            Transform::Axes::Local
        );
        cameraTransform.rotate(
            mouseXDelta * CAMERA_SENSITIVITY_YAW * secondsDelta,
            glm::vec3(0.0f, -1.0f, 0.0f),
            Transform::Axes::Local
        );

        float rollRate = 0.0f;
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        {
            rollRate = rollRate + 1.0f;
        }
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        {
            rollRate = rollRate - 1.0f;
        }
        cameraTransform.rotate(
            rollRate * CAMERA_SPEED_ROLL * secondsDelta,
            glm::vec3(0.0f, 0.0f, 1.0f),
            Transform::Axes::Local
        );

        // Calculate the view matrix
        glm::mat4 matrixView = cameraTransform.getMatrixInv();

        glClearColor(0.5f, 0.8f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for (int i = 0; i < countTransform; i++)
        {
            // Calculate the aggregate matrix
            camiCubes[i].update(secondsDelta);
            glm::mat4 matrix = matrixProject * matrixView * camiCubes[i].getMatrix();
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

double secondsSinceEpoch()
{
    auto now = std::chrono::system_clock::now();
    std::chrono::duration<double> ds = now.time_since_epoch();
    return ds.count();
}