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

constexpr float cameraSpeedTranslation = 5.0f;
constexpr float cameraSensitivityPitch = 0.005f;
constexpr float cameraSensitivityYaw = 0.005f;
constexpr float cameraSpeedRoll = 1.0f;

constexpr uint camiCubeCountMax = 100000;

constexpr int randomSeed = 11141997;

GLFWwindow* setupGl(void);
double secondsSinceEpoch(void);

void populateCamiCubes(
    CamiCubeSystem& system,
    uint count
);

void updatePlayerTransform(
    Transform& transform,
    GLFWwindow* window,
    const float& secondsDelta,
    const double& mouseXDelta,
    const double& mouseYDelta
);

int main(void)
{

    GLuint error;

    // Setup code
    GLFWwindow* window = setupGl();
    glfwMakeContextCurrent(window);
    glewInit();
    
    CamiCubeSystem camiCubeSystem(camiCubeCountMax);
    populateCamiCubes(camiCubeSystem, camiCubeCountMax);

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
    glEnable(GL_DEPTH_TEST);
    glfwSwapInterval(0);

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

    double timer1Second = 0;
    uint frameCounter = 0;

    while(glfwWindowShouldClose(window) == GL_FALSE) {

        error = glGetError();

        secondsLast = secondsNow;
        secondsNow = secondsSinceEpoch();
        secondsDelta = secondsNow - secondsLast;

        glfwPollEvents();
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window, GL_TRUE);
        }

        mouseXLast = mouseXNow;
        mouseYLast = mouseYNow;
        glfwGetCursorPos(window, &mouseXNow, &mouseYNow);
        mouseXDelta = mouseXNow - mouseXLast;
        mouseYDelta = mouseYNow - mouseYLast;
        updatePlayerTransform(
            cameraTransform,
            window,
            secondsDelta,
            mouseXDelta,
            mouseYDelta
        );

        // Calculate the view matrix
        glm::mat4 matrixView = cameraTransform.getMatrixInv();
        glm::mat4 matrixProjView = matrixProject * matrixView;

        camiCubeSystem.setMatrixProjView(matrixProjView);
        error = glGetError();
        camiCubeSystem.update(secondsDelta);
        error = glGetError();

        glClearColor(0.5f, 0.8f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        error = glGetError();

        camiCubeSystem.draw();
        error = glGetError();

        glfwSwapBuffers(window);

        timer1Second = timer1Second + secondsDelta;
        frameCounter = frameCounter + 1;
        if (timer1Second >= 1)
        {
            timer1Second = 0;
            printf("Frames in last 1 second:\t%i\n", frameCounter);
            frameCounter = 0;
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

void populateCamiCubes(
    CamiCubeSystem& system,
    uint count
)
{
    RandomGenerator randomGenerator(11141997);
    for (int i = 0; i < count; i++)
    {
        glm::vec3 position = randomGenerator.getUnitBall() * CLOUD_RADIUS;
        float orientationAngle = randomGenerator.getPositiveFloat() * M_PI;
        glm::vec3 orientationAxis = randomGenerator.getUnitSphere();
        glm::quat orientation = glm::angleAxis(orientationAngle, orientationAxis);
        float rotationRate = randomGenerator.getPositiveFloat() * CLOUD_ROTATION_RATE_MAX;
        glm::vec3 rotationAxis = randomGenerator.getUnitSphere();
        float scale = 0.5f;
        
        system.insert(
            scale,
            orientationAngle,
            orientationAxis,
            rotationRate,
            rotationAxis,
            position
        );
    }
}

void updatePlayerTransform(
    Transform& transform,
    GLFWwindow* window,
    const float& secondsDelta,
    const double& mouseXDelta,
    const double& mouseYDelta
)
{
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
    transform.translate(
        cameraSpeedTranslation * secondsDelta,
        cameraDirection,
        Transform::Axes::Local
    );

    transform.rotate(
        mouseYDelta * cameraSensitivityPitch,
        glm::vec3(-1.0f, 0.0f, 0.0f),
        Transform::Axes::Local
    );
    transform.rotate(
        mouseXDelta * cameraSensitivityYaw,
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
    transform.rotate(
        rollRate * cameraSpeedRoll * secondsDelta,
        glm::vec3(0.0f, 0.0f, 1.0f),
        Transform::Axes::Local
    );
}

double secondsSinceEpoch()
{
    auto now = std::chrono::system_clock::now();
    std::chrono::duration<double> ds = now.time_since_epoch();
    return ds.count();
}