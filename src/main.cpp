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
#include "camiCube.hpp"
#include "randomGeneration.hpp"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define WINDOW_TITLE "Cami Cube"

#define CLOUD_RADIUS 80.0f
#define CLOUD_ROTATION_RATE_MAX 1.0f

#define CAMERA_SPEED_TRANSLATION 5.0f
#define CAMERA_SENSITIVITY_PITCH 0.1f
#define CAMERA_SENSITIVITY_YAW   0.1f
#define CAMERA_SPEED_ROLL        1.0f

constexpr uint camiCubeCount = 100000;

GLFWwindow* setupGl(void);
double secondsSinceEpoch(void);

GLuint createCubeMesh(void);

class Lightbulb
{

private:
    glm::vec3 center_;
    glm::vec3 radius_;
    glm::vec3 axis_;
    float rate_;
    float angle_;
    float scale_;

public:

    Lightbulb(void) :
        center_(0.0f, 0.0f, 0.0f),
        radius_(0.0f, 0.0f, 0.0f),
        axis_(0.0f, 0.0f, 0.0f),
        rate_(0.0f),
        angle_(0.0f),
        scale_(1.0f)
    {}

    Lightbulb(
        const glm::vec3& center,
        const glm::vec3& starting,
        const glm::vec3& axis,
        const float& rate,
        const float& scale
    ) :
        center_(center),
        radius_(starting - center),
        axis_(glm::normalize(axis)),
        rate_(rate),
        angle_(0),
        scale_(scale)
    {}

    void update(
        const float& secondsDelta
    )
    {
        angle_ = angle_ + rate_ * secondsDelta;
    }

    glm::mat4 getMatrix(void) const
    {
        glm::vec3 rotatedRadius = glm::angleAxis(angle_, axis_) * radius_;
        glm::vec3 position = rotatedRadius + center_;
        glm::mat4 matrix = glm::identity<glm::mat4>();
        matrix = glm::translate(matrix, position);
        matrix = glm::scale(matrix, glm::vec3(scale_));
        return matrix;
    }

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
    glBindTexture(GL_TEXTURE_2D, 0);
    error = glGetError();

    // Load Cube mesh
    GLuint vaoCube = createCubeMesh();

    // Load and configure shader
    GLuint shaderProgram = compileShader(
        &shaderVertexSource, 1,
        &shaderFragmentSource, 1
    );
    
    RandomGenerator randomGenerator(11141997);
    CamiCubeSystem camiCubeSystem(camiCubeCount);
    for (int i = 0; i < camiCubeCount; i++)
    {
        glm::vec3 position = randomGenerator.getUnitBall() * CLOUD_RADIUS;
        float orientationAngle = randomGenerator.getPositiveFloat() * M_PI;
        glm::vec3 orientationAxis = randomGenerator.getUnitSphere();
        glm::quat orientation = glm::angleAxis(orientationAngle, orientationAxis);
        float rotationRate = randomGenerator.getPositiveFloat() * CLOUD_ROTATION_RATE_MAX;
        glm::vec3 rotationAxis = randomGenerator.getUnitSphere();
        float scale = 0.5f;
        
        camiCubeSystem.insert(
            position,
            orientation,
            scale,
            rotationAxis,
            rotationRate
        );
    }

    // Array of model matrices for CamiCubes
    glm::mat4* camiCubeMatrices = new glm::mat4[camiCubeCount];

    // Create one-pixel lightbulb texture
    GLuint textureLightbulb;
    glGenTextures(1, &textureLightbulb);
    glBindTexture(GL_TEXTURE_2D, textureLightbulb);
    glm::vec4 colorLightbulb = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGBA,
        1,
        1,
        0,
        GL_RGBA,
        GL_FLOAT,
        &colorLightbulb
    );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);

    // Create a lightbulb
    Lightbulb lightbulb = Lightbulb(
        glm::vec3(6.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, -1.0f, 0.0f),
        0.5f,
        0.1f
    );

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
        10000.0f
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

        camiCubeSystem.update(secondsDelta);

        // Calculate the view matrix
        glm::mat4 matrixView = cameraTransform.getMatrixInv();

        uint camiCubeMatrixCount = camiCubeSystem.getMatrices(
            camiCubeMatrices,
            1,
            camiCubeCount
        );

        glClearColor(0.5f, 0.8f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBindTexture(GL_TEXTURE_2D, textureCami);
        for (int i = 0; i < camiCubeCount; i++)
        {
            // Calculate the aggregate matrix
            glm::mat4 matrix = matrixProject * matrixView * camiCubeMatrices[i];
            glUniformMatrix4fv(
                0,
                1,
                GL_FALSE,
                glm::value_ptr(matrix)
            );

            glDrawElements(
                GL_TRIANGLES,
                36,
                GL_UNSIGNED_INT,
                0
            );
            error = glGetError();
        }

        glBindTexture(GL_TEXTURE_2D, textureLightbulb);
        lightbulb.update(secondsDelta);
        glm::mat4 matrix = matrixProject * matrixView * lightbulb.getMatrix();
        glUniformMatrix4fv(
            0,
            1,
            GL_FALSE,
            glm::value_ptr(matrix)
        );
        glDrawElements(
            GL_TRIANGLES,
            36,
            GL_UNSIGNED_INT,
            0
        );
        error = glGetError();

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

// Creates and binds a cubical mesh with texture coordinates. Returns the
// vao handle.
GLuint createCubeMesh(void)
{

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

    return vaoCube;

}

double secondsSinceEpoch()
{
    auto now = std::chrono::system_clock::now();
    std::chrono::duration<double> ds = now.time_since_epoch();
    return ds.count();
}