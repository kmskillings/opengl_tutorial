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
#include "meshSphere.hpp"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define WINDOW_TITLE "Cami Cube"

#define CLOUD_RADIUS 40.0f
#define CLOUD_ROTATION_RATE_MAX 1.0f

constexpr float cameraSpeedTranslation = 5.0f;
constexpr float cameraSensitivityPitch = 0.005f;
constexpr float cameraSensitivityYaw = 0.005f;
constexpr float cameraSpeedRoll = 1.0f;
constexpr float cameraStandoff = 2.0f;

constexpr uint camiCubeCountMax = 10000;

constexpr int randomSeed = 11141997;

GLFWwindow* setupGl(void);
double secondsSinceEpoch(void);

void populateCamiCubes(
    CamiCubeSystem& system,
    uint count
);

glm::quat updatePlayerOrientation(
    const glm::quat& playerOrientation,
    GLFWwindow* window,
    const float& secondsDelta,
    const double& mouseXDelta,
    const double& mouseYDelta
);

glm::vec3 updatePlayerPosition(
    const glm::vec3& position,
    const glm::quat& orientation,
    GLFWwindow* window,
    const float& secondsDelta
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

    glm::quat cameraOrientation = glm::angleAxis(
        0.0f, 
        glm::vec3(1.0f, 0.0f, 0.0f)
    );
    glm::vec3 spherePosition = glm::vec3(0.0f, 0.0f, 0.0f);

    GLuint sphereShader = compileShader(
        &sphereVertexSource, 1,
        &sphereFragmentSource, 1
    );
    MeshSphere meshSphere(16, 32, 0.5f);

    GLuint highlightVao;
    glGenVertexArrays(1, &highlightVao);
    GLuint highlightVbo;
    glGenBuffers(1, &highlightVbo);
    glBindVertexArray(highlightVao);
    glBindBuffer(GL_ARRAY_BUFFER, highlightVbo);
    float highlightVertices[] = {
        -1.0f, -1.0f,
         1.0f, -1.0f,
        -1.0f,  1.0f,
         1.0f,  1.0f
    };
    glBufferData(
        GL_ARRAY_BUFFER,
        8 * sizeof(float),
        highlightVertices,
        GL_STATIC_DRAW
    );
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        0,
        2,
        GL_FLOAT,
        GL_FALSE,
        2 * sizeof(float),
        (void*)0
    );
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    GLuint highlightEbo;
    glGenBuffers(1, &highlightEbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, highlightEbo);
    uint highlightTriangles[] = {
        0, 1, 2,
        2, 1, 3
    };
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        6 * sizeof(uint),
        highlightTriangles,
        GL_STATIC_DRAW
    );
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    GLuint highlightShader = compileShader(
        &highlightVertexSource, 1,
        &highlightFragmentSource, 1
    );

    // Calculate project matrix
    glm::mat4 matrixProject = glm::perspective(
        (float)(M_PI / 3.0f),
        (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT,
        0.1f,
        10000.0f
    );

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSwapInterval(1);

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
        cameraOrientation = updatePlayerOrientation(
            cameraOrientation,
            window,
            secondsDelta,
            mouseXDelta,
            mouseYDelta
        );
        spherePosition = updatePlayerPosition(
            spherePosition,
            cameraOrientation,
            window,
            secondsDelta
        );

        // Calculate the view matrix
        glm::mat4 matrixRotate = glm::mat4_cast(cameraOrientation);
        glm::mat4 matrixStandoff = glm::translate(
            glm::mat4(1.0f), 
            glm::vec3(0.0f, 0.0f, cameraStandoff)
        );
        glm::mat4 matrixPosition = glm::translate(
            glm::mat4(1.0f),
            spherePosition
        );
        glm::mat4 matrixView = glm::inverse(
            matrixPosition * 
            matrixRotate * 
            matrixStandoff
        );

        camiCubeSystem.setMatrices(matrixProject, matrixView);
        camiCubeSystem.setCutoutSphere(
            spherePosition,
            0.5f
        );
        camiCubeSystem.update(secondsDelta);

        glm::mat4 matrixSphere = matrixProject * matrixView * matrixPosition;

        glClearColor(0.5f, 0.8f, 1.0f, 1.0f);
        glClearStencil(0);
        glClear(
            GL_COLOR_BUFFER_BIT | 
            GL_DEPTH_BUFFER_BIT | 
            GL_STENCIL_BUFFER_BIT
        );

        // Draw the Cami cubes
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
        glEnable(GL_DEPTH_TEST);
        glDepthMask(GL_TRUE);
        glDepthFunc(GL_LESS);
        glDisable(GL_STENCIL_TEST);
        // glEnable(GL_CULL_FACE);
        // glCullFace(GL_BACK);
        camiCubeSystem.draw();

        // Draw the wireframe sphere over everything else
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
        glEnable(GL_DEPTH_TEST);
        glDepthMask(GL_TRUE);
        glDepthFunc(GL_LESS);
        glDisable(GL_STENCIL_TEST);
        // glEnable(GL_CULL_FACE);
        // glCullFace(GL_BACK);
        glUseProgram(sphereShader);
        glUniformMatrix4fv(
            0,
            1,
            GL_FALSE,
            glm::value_ptr(matrixSphere)
        );
        glBindVertexArray(meshSphere.getVao());
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshSphere.getEboLines());
        glDrawElements(GL_LINES, 2 * meshSphere.getCountLines(), GL_UNSIGNED_INT, (void*)0);

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

glm::quat updatePlayerOrientation(
    const glm::quat& playerOrientation,
    GLFWwindow* window,
    const float& secondsDelta,
    const double& mouseXDelta,
    const double& mouseYDelta
)
{
    glm::quat pitch = glm::angleAxis(
        (float)mouseYDelta * cameraSensitivityPitch,
        glm::vec3(-1.0f, 0.0f, 0.0f)
    );

    glm::quat yaw = glm::angleAxis(
        (float)mouseXDelta * cameraSensitivityYaw,
        glm::vec3(0.0f, -1.0f, 0.0f)
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
    glm::quat roll = glm::angleAxis(
        rollRate * cameraSpeedRoll * secondsDelta,
        glm::vec3(0.0f, 0.0f, 1.0f)
    );

    return playerOrientation * pitch * yaw * roll;
}

glm::vec3 updatePlayerPosition(
    const glm::vec3& position,
    const glm::quat& orientation,
    GLFWwindow* window,
    const float& secondsDelta
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
    cameraDirection = orientation * cameraDirection;
    return glm::vec3(position) + cameraDirection * cameraSpeedTranslation * secondsDelta;
}

double secondsSinceEpoch()
{
    auto now = std::chrono::system_clock::now();
    std::chrono::duration<double> ds = now.time_since_epoch();
    return ds.count();
}