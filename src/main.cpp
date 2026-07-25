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

#include "world.hpp"
#include "camiCubeVao.hpp"
#include "meshSphere.hpp"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define WINDOW_TITLE "Cami Cube"

constexpr float cloudRadius = 40.0f;
#define CLOUD_ROTATION_RATE_MAX 1.0f
constexpr uint averageCamiCubesPerChunk = 10;

constexpr float cameraSpeedTranslation = 5.0f;
constexpr float cameraSensitivityPitch = 0.005f;
constexpr float cameraSensitivityYaw = 0.005f;
constexpr float cameraSpeedRoll = 1.0f;
constexpr float cameraStandoff = 2.0f;

constexpr uint maxCollisions = 10;

constexpr float throbPeriod = 1.0f;

constexpr uint camiCubeCountMax = 10000;

constexpr int randomSeed = 11141997;

GLFWwindow* setupGl(void);
double secondsSinceEpoch(void);

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

    World world(
        camiCubeCountMax,
        cloudRadius,
        averageCamiCubesPerChunk
    );

    CamiCubeVao camiCubeVao(
        world.getCamiCubeCount(),
        world.getCamiCubePositions(),
        world.getCamiCubeOrientations()
    );

    MeshSphere meshSphere(16, 32, 0.5f);

    GLuint camiCubeShader = compileShader(
        &camiCubeVertexSource, 1,
        &camiCubeFragmentSource, 1
    );

    GLuint sphereShader = compileShader(
        &sphereVertexSource, 1,
        &sphereFragmentSource, 1
    );
    
    GLuint camiCubeTexture;
    glGenTextures(1, &camiCubeTexture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, camiCubeTexture);
    glTexImage2D(
        GL_TEXTURE_2D, 
        0, 
        GL_BGRA, 
        camiTextureWidth, 
        camiTextureHeight, 
        0, 
        GL_BGRA, 
        GL_UNSIGNED_BYTE, 
        camiTextureBytes
    );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // glBindTexture(GL_TEXTURE_2D, 0);

    glm::quat cameraOrientation = glm::angleAxis(
        0.0f, 
        glm::vec3(1.0f, 0.0f, 0.0f)
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
    double secondsElapsed = 0;

    while(glfwWindowShouldClose(window) == GL_FALSE) {

        secondsLast = secondsNow;
        secondsNow = secondsSinceEpoch();
        secondsDelta = secondsNow - secondsLast;
        secondsElapsed = secondsElapsed + secondsDelta;

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
        world.setSpherePosition(updatePlayerPosition(
            world.getSpherePosition(),
            cameraOrientation,
            window,
            secondsDelta
        ));

        // Calculate the view matrix
        glm::mat4 matrixRotate = glm::mat4_cast(cameraOrientation);
        glm::mat4 matrixStandoff = glm::translate(
            glm::mat4(1.0f), 
            glm::vec3(0.0f, 0.0f, cameraStandoff)
        );
        glm::mat4 matrixPosition = glm::translate(
            glm::mat4(1.0f),
            world.getSpherePosition()
        );
        glm::mat4 matrixView = glm::inverse(
            matrixPosition * 
            matrixRotate * 
            matrixStandoff
        );

        glm::mat4 matrixProjView = matrixProject * matrixView;

        glm::mat4 matrixSpherePosition = glm::translate(glm::mat4(1.0f), world.getSpherePosition());
        glm::mat4 matrixSphere = matrixProjView * matrixSpherePosition;

        // Rendering
        // Clear the buffer
        glClearColor(0.5f, 0.8f, 1.0f, 1.0f);
        glClear(
            GL_COLOR_BUFFER_BIT |
            GL_DEPTH_BUFFER_BIT
        );

        // Draw the Cami Cubes

        glUseProgram(camiCubeShader);
        glUniformMatrix4fv(
            0,
            1,
            GL_FALSE,
            glm::value_ptr(matrixProjView)
        );
        glUniform1f(
            1,
            secondsElapsed
        );
        glUniform1i(
            2,
            0
        );
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
        glEnable(GL_DEPTH_TEST);
        glDepthMask(GL_TRUE);
        glDepthFunc(GL_LESS);
        glDisable(GL_STENCIL_TEST);
        glBindVertexArray(camiCubeVao.getVao());
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, camiCubeVao.getEbo());
        glDrawElementsInstanced(
            GL_TRIANGLES,
            3 * camiCubeVao.getTriangleCount(),
            GL_UNSIGNED_INT,
            (void*)0,
            camiCubeVao.getInstanceCount()
        );

        // Draw the sphere

        glUseProgram(sphereShader);
        glUniformMatrix4fv(
            0,
            1,
            GL_FALSE,
            glm::value_ptr(matrixSphere)
        );
        glBindVertexArray(meshSphere.getVao());
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshSphere.getEboLines());
        glDrawElements(
            GL_LINES,
            2 * meshSphere.getCountLines(),
            GL_UNSIGNED_INT,
            (void*)0
        );

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